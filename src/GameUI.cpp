#include "GameUI.h"
#include "NullDrawable.h"
#include "HollowRectangle.h"
#include "PieceI.h"

namespace gui {

Uint32 timer_callback(Uint32 interval, void* param);

GameUI::GameUI(SDL_Window* window, GLint mul, int _width, int _height) :
  sdl_window(window),
  model_uniform_location(mul),
  root(0),
  active_piece(0),
  block_area_width(1.0f),
  block_area_height(2.0f),
  width(_width),
  height(_height) {

  root = new gfx::NullDrawable;
  allocated_drawables.insert(root);

  gfx::HollowRectangle* block_area_outline = new gfx::HollowRectangle(
    model_uniform_location, glm::vec2(0.5f, 1.0f),
    block_area_width / 2.0f, block_area_height / 2.0f,
    glm::vec3(0.0f, 1.0f, 0.0f));
  root->addDrawable(block_area_outline);
  allocated_drawables.insert(block_area_outline);

  gfx::NullDrawable* block_scale_area = new gfx::NullDrawable(
    glm::scale(glm::mat4(), glm::vec3(1.0f / 12.0f, 1.0f / 12.0f, 1.0f)));
  block_area_outline->addDrawable(block_scale_area);
  allocated_drawables.insert(block_scale_area);

  gfx::PieceI* iPiece = new gfx::PieceI(model_uniform_location, 0, 12);
  block_scale_area->addDrawable(iPiece);
  allocated_drawables.insert(iPiece);
  active_piece = iPiece;
}

GameUI::~GameUI() {
  for(std::set<gfx::DrawableHierarchy*>::iterator it = allocated_drawables.begin();
      it != allocated_drawables.end(); ++it) {
    delete *it;
  }
}

void GameUI::run() {
  Uint32 update_delay_ms = 1000;
  Uint32 update_position_code = static_cast<Uint32>(UPDATE_POSITION);
  SDL_AddTimer(update_delay_ms, timer_callback, &update_position_code);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gfx::TransformStack ts;
    root->renderHierarchy(ts);

    SDL_GL_SwapWindow(sdl_window);
    SDL_Delay(20);
  } while(handle_user_input());
}

bool GameUI::handle_user_input() {
  bool is_quit = false;
  SDL_Event sdl_event;
  while(SDL_PollEvent(&sdl_event)) {
    switch(sdl_event.type) {
    case SDL_QUIT: {
      is_quit = true;
      break;
    }

    case SDL_KEYUP: {
      if(sdl_event.key.keysym.sym == SDLK_ESCAPE) {
	is_quit = true;
      }
      break;
    }

    case SDL_KEYDOWN: {
      if(sdl_event.key.keysym.sym == SDLK_LEFT) {
	active_piece->moveLeft();
      }
      else if(sdl_event.key.keysym.sym == SDLK_RIGHT) {
	active_piece->moveRight();
      }
      else if(sdl_event.key.keysym.sym == SDLK_DOWN) {
	active_piece->moveDown();
      }
      else if(sdl_event.key.keysym.sym == SDLK_UP) {
	active_piece->rotate();
      }
      break;
    }

    case SDL_USEREVENT: {
      if(sdl_event.user.code == UPDATE_POSITION) {
	active_piece->moveDown();
      }
      break;
    }

    default: {
      break;
    }
    }
  }

  return !is_quit;
}

Uint32 timer_callback(Uint32 interval, void* param) {
  SDL_Event event;
  SDL_memset(&event, 0, sizeof(event));
  event.type = SDL_USEREVENT;
  event.user.code = *reinterpret_cast<Uint32*>(param);
  SDL_PushEvent(&event);
  return interval;
}

}
