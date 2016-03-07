#include "GameUI.h"
#include "HollowRectangle.h"
#include "PieceI.h"
#include <string.h>
#include <iostream>
#include <sstream>

namespace gui {

static const Uint32 BASE_DELAY_MS = 1000;
static const Uint32 KEYPRESS_DELAY_MS = 60;

Uint32 block_move_timer_callback(Uint32 interval, void* param);
Uint32 keypress_timer_callback(Uint32 interval, void* param);

GameUI::GameUI(SDL_Window* window, int _width, int _height) :
  sdl_window(window),
  model_mat_location(0),
  root(0),
  active_piece(0),
  block_area_width(1.0f),
  block_area_height(2.0f),
  width(_width),
  height(_height),
  grid(0),
  block_scale_area(0),
  font_shader_program(0),
  default_shader_program(0),
  score(0),
  level(1),
  game_start_ticks(0),
  last_clear_was_a_tetris(false),
  mutex(0),
  block_move_timer_id(0),
  keypress_timer_id(0),
  is_game_over(false) {

  memset(keys_pressed, 0, sizeof(keys_pressed));
  mutex = SDL_CreateMutex();

  {
    dsp::NullPreLinkCallback plc;
    if(!dsp::compile_shaders("shaders/font_shader.vert", "shaders/font_shader.frag",
			     font_shader_program, &plc)) {
      std::cerr << "Error compiling font shader" << std::endl;
      return;
    }

    if(!dsp::compile_shaders("shaders/shader.vert", "shaders/shader.frag",
			     default_shader_program, &plc)) {
      std::cerr << "Error compiling default shader" << std::endl;
      return;
    }
  }

  glm::mat4 ident_mat;
  glm::mat4 view_mat = glm::translate(glm::mat4(), glm::vec3(-0.5f, -1.0f, -3.0f));
  glm::mat4 proj_mat = glm::perspective(
    glm::radians(45.0f),
    static_cast<float>(width) / static_cast<float>(height),
    0.1f, 100.0f);

  // Set initial uniforms on the font shader
  GLint proj_mat_location = glGetUniformLocation(font_shader_program, "proj_mat");
  dsp::checkGL();
  GLint view_mat_location = glGetUniformLocation(font_shader_program, "view_mat");
  dsp::checkGL();
  glUseProgram(font_shader_program);
  dsp::checkGL();
  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));
  dsp::checkGL();
  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));
  dsp::checkGL();

  // Set initial uniforms on the default shader
  proj_mat_location = glGetUniformLocation(default_shader_program, "proj_mat");
  dsp::checkGL();
  model_mat_location = glGetUniformLocation(default_shader_program, "model_mat");
  dsp::checkGL();
  view_mat_location = glGetUniformLocation(default_shader_program, "view_mat");
  dsp::checkGL();
  glUseProgram(default_shader_program);
  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));
  dsp::checkGL();
  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));
  dsp::checkGL();
  glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, glm::value_ptr(ident_mat));
  dsp::checkGL();
  dsp::printAll(default_shader_program);


  root = new gfx::NullDrawable;
  allocated_drawables.insert(root);

  gfx::HollowRectangle* block_area_outline = new gfx::HollowRectangle(
    model_mat_location, glm::vec2(0.5f, 1.0f),
    block_area_width / 2.0f, block_area_height / 2.0f,
    glm::vec3(0.0f, 1.0f, 0.0f));
  root->addDrawable(block_area_outline);
  allocated_drawables.insert(block_area_outline);

  block_scale_area = new gfx::NullDrawable(
    glm::scale(glm::mat4(), glm::vec3(1.0f / 12.0f, 1.0f / 12.0f, 1.0f)));
  block_area_outline->addDrawable(block_scale_area);
  allocated_drawables.insert(block_scale_area);

  gfx::PieceI* iPiece = new gfx::PieceI(model_mat_location, 0, 12);
  block_scale_area->addDrawable(iPiece);
  allocated_drawables.insert(iPiece);
  active_piece = iPiece;

  grid = new gfx::Grid(12, 24);
  allocated_drawables.insert(grid);
  block_scale_area->addDrawable(grid);
}

GameUI::~GameUI() {
  SDL_LockMutex(mutex);
  SDL_RemoveTimer(block_move_timer_id);
  SDL_RemoveTimer(keypress_timer_id);

  for(std::set<gfx::DrawableHierarchy*>::iterator it = allocated_drawables.begin();
      it != allocated_drawables.end(); ++it) {
    delete *it;
  }
  SDL_UnlockMutex(mutex);
  SDL_DestroyMutex(mutex);
}

void GameUI::run() {
  gfx::TextRenderer textRendering(font_shader_program, "fonts/FreeMono.ttf");

  block_move_timer_id = SDL_AddTimer(BASE_DELAY_MS, block_move_timer_callback, this);
  keypress_timer_id = SDL_AddTimer(KEYPRESS_DELAY_MS, keypress_timer_callback, 0);
  SDL_LockMutex(mutex);
  game_start_ticks = SDL_GetTicks();
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gfx::TransformStack ts;
    glUseProgram(default_shader_program);
    root->renderHierarchy(ts);

    glUseProgram(font_shader_program);
    renderTextBoxes(textRendering);

    SDL_GL_SwapWindow(sdl_window);
    SDL_UnlockMutex(mutex);
    SDL_Delay(20);
    SDL_LockMutex(mutex);
  } while(handle_user_input());
  SDL_UnlockMutex(mutex);
}

Uint32 GameUI::blockMoveTimerCallback() {
  SDL_LockMutex(mutex);

  level = 1 + (SDL_GetTicks() - game_start_ticks) / 30000;

  {
    std::ostringstream ost;
    ost << "SDLT: " << SDL_GetTicks();
    debug_text = ost.str();
  }

  SDL_UnlockMutex(mutex);

  SDL_Event event;
  SDL_memset(&event, 0, sizeof(event));
  event.type = SDL_USEREVENT;
  event.user.code = UPDATE_POSITION;
  SDL_PushEvent(&event);

  return BASE_DELAY_MS / level;
}

bool GameUI::handle_user_input() {
  bool is_quit = false;
  bool is_keypress_event = false;
  SDL_Event sdl_event;
  while(SDL_PollEvent(&sdl_event) && !is_quit) {
    switch(sdl_event.type) {
    case SDL_QUIT: {
      is_quit = true;
      break;
    }

    case SDL_KEYUP: {
      if(sdl_event.key.keysym.sym == SDLK_ESCAPE) {
	is_quit = true;
      }
      else if(sdl_event.key.keysym.sym == SDLK_LEFT) {
	keys_pressed[LEFT] = 0;
      }
      else if(sdl_event.key.keysym.sym == SDLK_RIGHT) {
	keys_pressed[RIGHT] = 0;
      }
      else if(sdl_event.key.keysym.sym == SDLK_DOWN) {
	keys_pressed[DOWN] = 0;
      }
      break;
    }

    case SDL_KEYDOWN: {
      if(sdl_event.key.keysym.sym == SDLK_LEFT) {
	keys_pressed[LEFT] = 1;
      }
      else if(sdl_event.key.keysym.sym == SDLK_RIGHT) {
	keys_pressed[RIGHT] = 1;
      }
      else if(sdl_event.key.keysym.sym == SDLK_DOWN) {
	keys_pressed[DOWN] = 1;
      }
      else if(sdl_event.key.keysym.sym == SDLK_UP) {
	active_piece->rotate(grid);
      }
      break;
    }

    case SDL_USEREVENT: {
      if(sdl_event.user.code == UPDATE_POSITION) {
	if(!active_piece->moveDown(grid)) {
	  is_quit = initNewPieceAndScanForGameEnd();
	}
      }
      else if(sdl_event.user.code == KEYPRESS_UPDATE) {
	is_keypress_event = true;
      }
      break;
    }

    default: {
      break;
    }
    }
  }

  if(is_keypress_event) {
    if(keys_pressed[DOWN] == 1) {
      if(!active_piece->moveDown(grid)) {
	is_quit = initNewPieceAndScanForGameEnd();
      }
    }
    if(keys_pressed[LEFT] == 1) {
      active_piece->moveLeft(grid);
    }
    if(keys_pressed[RIGHT] == 1) {
      active_piece->moveRight(grid);
    }
  }

  return !is_quit;
}

bool GameUI::initNewPieceAndScanForGameEnd() {
  if(!grid->addPieceSquaresToGrid(active_piece)) {
    is_game_over = true;
  }
  else {
    allocated_drawables.erase(active_piece);
    block_scale_area->removeDrawable(active_piece);
    delete active_piece;
    scanGridForMatches();
    if(!is_game_over) {
      active_piece = generateNewPiece();
      allocated_drawables.insert(active_piece);
      block_scale_area->addDrawable(active_piece);
    }
  }
  return is_game_over;
}

void GameUI::scanGridForMatches() {
  std::vector<int> fullLevels;
  bool isTetris = false;
  int numFullLevelsCleared = 0;
  do {
    // Make a list of rows that have all columns filled
    fullLevels.clear();
    for(int h = grid->getFirstRow(); h < grid->getEndRow(); h++) {
      int numOccupied = 0;
      for(int w = grid->getFirstColumn(); w < grid->getEndColumn(); w++) {
	if(grid->isSpaceOccupied(w, h)) {
	  numOccupied++;
	}
	else {
	  break;
	}
      }

      if(numOccupied == grid->getWidth()) {
	fullLevels.push_back(h);
      }
    }

    if(fullLevels.size() == 4) {
      isTetris = true;
    }
    else {
      numFullLevelsCleared += fullLevels.size();
    }

    for(std::vector<int>::const_iterator it = fullLevels.begin();
	it != fullLevels.end(); ++it) {
      for(int w = grid->getFirstColumn(); w < grid->getEndColumn(); ++w) {
	grid->clearSpaceAt(w, *it);
      }
    }

    bool squaresMovedDownward;
    do {
      // Find floating clusters of squares and move them downwards until they either
      // lie on top of another cluster, or lie at the bottom of the grid.
      std::set<gfx::Grid::Space> unclusteredSquares;
      grid->getFilledSpaces(unclusteredSquares);
      auto expandCluster = [&unclusteredSquares](std::set<gfx::Grid::Space>& cluster,
						 gfx::Grid::Space seed) -> void {
	std::list<gfx::Grid::Space> frontier;
	frontier.push_back(seed);
	const int neighboorhoodCoords[] = {-1, 0, 1};

	do {
	  gfx::Grid::Space space = frontier.front();
	  cluster.insert(space);
	  unclusteredSquares.erase(space);
	  frontier.pop_front();

	  for(int w : neighboorhoodCoords) {
	    for(int h : neighboorhoodCoords) {
	      // Skip corners and the center of the 3x3 box around the space;
	      // only consider the non-diagonal neighbors
	      if(abs(w) == abs(h)) {
		continue;
	      }

	      gfx::Grid::Space test = {
		0,
		space.x + w,
		space.y + h
	      };

	      std::set<gfx::Grid::Space>::const_iterator it = unclusteredSquares.find(test);
	      if(it != unclusteredSquares.end()) {
		frontier.push_back(*it);
	      }
	    }
	  }
	} while(!frontier.empty());
      };

      std::list<std::set<gfx::Grid::Space> > clusters;
      while(!unclusteredSquares.empty()) {
	clusters.push_back(std::set<gfx::Grid::Space>());
	expandCluster(clusters.back(), *unclusteredSquares.begin());
      }

      squaresMovedDownward = false;
      for(std::list<std::set<gfx::Grid::Space> >::iterator it = clusters.begin();
	  it != clusters.end(); ++it) {

	// Test all member squares of the cluster to see if they all can move down
	// by one square. They can move down by one square if all the following
	// is true for every square in the cluster:
	// 1. They do not currently lie at the bottom of the grid
	// 2. The space below them is unoccupied, or is occupied by another member
	//    of the same cluster
	bool canMoveDownward = true;
	for(std::set<gfx::Grid::Space>::iterator cit = it->begin();
	    cit != it->end(); ++cit) {
	  gfx::Grid::Space oneBelow = {
	    0,
	    cit->x,
	    cit->y - 1
	  };

	  if(grid->isOutside(cit->x, cit->y - 1) ||
	     (grid->isSpaceOccupied(cit->x, cit->y - 1) &&
	      it->find(oneBelow) == it->end())) {
	    canMoveDownward = false;
	    break;
	  }
	}

	if(canMoveDownward) {
	  squaresMovedDownward = true;
	  for(std::set<gfx::Grid::Space>::iterator cit = it->begin();
	      cit != it->end(); ++cit) {
	    gfx::GridSquare* gridSquare = grid->getGridSquareAt(cit->x, cit->y);
	    grid->clearSpaceAt(cit->x, cit->y);
	    grid->setSpace(gridSquare, cit->x, cit->y-1);
	  }
	}
      }
    } while(squaresMovedDownward);
  } while(!fullLevels.empty());

  // Scoring level clears as follows:
  // 160 points per BTB Tetris
  // 80 points per Tetris
  // 10 points per regular level clear
  if(isTetris) {
    if(last_clear_was_a_tetris) {
      incrementScoreByLevel(1600);
      notification_text = "B2B Tetris!";
    }
    else {
      incrementScoreByLevel(80);
      notification_text = "Tetris!";
    }
    notification_start_ticks = SDL_GetTicks();
  }
  last_clear_was_a_tetris = isTetris;
  incrementScoreByLevel(numFullLevelsCleared * 10);
}

gfx::ActivePiece* GameUI::generateNewPiece() {
  return new gfx::PieceI(model_mat_location, 0, 12);
}

void GameUI::incrementScoreByLevel(int basePoints) {
  float multiplier = 1.0f;
  if(level < 5) {
    multiplier = level * 1.5f;
  }
  else if(level < 10) {
    multiplier = level * 1.3;
  }
  else {
    multiplier = level * 1.2;
  }
  score += static_cast<int>(multiplier * basePoints);
}

void GameUI::renderTextBoxes(gfx::TextRenderer& textRenderer) {
  const float scale = 0.001f;
  const glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

  std::ostringstream scoreString;
  scoreString << "Score: " << score;
  textRenderer.renderText(scoreString.str(),
			   0.0f, -0.1f, scale, color);

  std::ostringstream levelString;
  levelString << "Level: " << level;
  textRenderer.renderText(levelString.str(),
			   0.0f, -0.15f, scale, color);

  if(SDL_GetTicks() > 0 &&
     SDL_GetTicks() - notification_start_ticks < 2000) {
    textRenderer.renderText(notification_text,
			    0.4f,
			    2.1f,
			    scale * 2.0f,
			    color);
  }

  if(debug_text.size() > 0) {
    textRenderer.renderText(debug_text,
			    0.7f,
			    -0.1f,
			    scale,
			    color);
  }
}

Uint32 block_move_timer_callback(Uint32 interval, void* param) {
  GameUI* instance = reinterpret_cast<GameUI*>(param);
  return instance->blockMoveTimerCallback();
}

Uint32 keypress_timer_callback(Uint32 interval, void* param) {
  SDL_Event event;
  SDL_memset(&event, 0, sizeof(event));
  event.type = SDL_USEREVENT;
  event.user.code = gui::GameUI::KEYPRESS_UPDATE;
  SDL_PushEvent(&event);
  return interval;
}


}
