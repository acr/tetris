#include "gl_defns.h"
#include "gl_utils.h"

#include <SDL2/SDL_thread.h>
#include "TransformStack.h"
#include "DrawableHierarchy.h"
#include "NullDrawable.h"
#include "HollowRectangle.h"
#include "FilledSquare.h"
#include "ActivePiece.h"
#include "PieceI.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <queue>

static const int width = 512;
static const int height = 1024;
static Uint32 update_event_type = 0;

using namespace gfx;

class PLC : public dsp::PreLinkCallback {
public:
  virtual ~PLC() {}
  virtual void preLinkCallback(GLint shader_program) { }
};

bool handle_user_input(ActivePiece* piece);
Uint32 timer_callback(Uint32 interval, void* param);

int main() {
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow("nt",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					width,
					height,
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if(window == 0) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return 2;
  }


  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, glContext);

  glewExperimental = true;
  GLenum glewInitStatus = glewInit();
  if(glewInitStatus != 0) {
    std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitStatus) << std::endl;
    return 3;
  }

  glGetError();
  dsp::printGLInfo();

  glDepthFunc(GL_NEVER);	// Ignore Z depth values, so that objects are drawn back to front
  dsp::checkGL();
  glEnable(GL_CULL_FACE);
  dsp::checkGL();
  glCullFace(GL_BACK);
  dsp::checkGL();
  glFrontFace(GL_CCW);
  dsp::checkGL();

  GLint shader_program;
  {
    PLC plc;
    if(!dsp::compile_shaders("shaders/shader.vert", "shaders/shader.frag",
			     shader_program, &plc)) {
      std::cerr << "Error compiling shader program" << std::endl;
      return 4;
    }
  }

  glm::mat4 ident_mat;
  glm::mat4 view_mat = glm::translate(glm::mat4(), glm::vec3(-0.5f, -1.0f, -3.0f));
  glm::mat4 proj_mat = glm::perspective(
    glm::radians(45.0f),
    static_cast<float>(width) / static_cast<float>(height),
    0.1f, 100.0f);

  GLint proj_mat_location = glGetUniformLocation(shader_program, "proj_mat");
  dsp::checkGL();
  GLint model_mat_location = glGetUniformLocation(shader_program, "model_mat");
  dsp::checkGL();
  GLint view_mat_location = glGetUniformLocation(shader_program, "view_mat");
  dsp::checkGL();
  glUseProgram(shader_program);
  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));
  dsp::checkGL();
  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));
  dsp::checkGL();
  glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, glm::value_ptr(ident_mat));
  dsp::checkGL();
  dsp::printAll(shader_program);

  NullDrawable root;
  const float block_area_width = 1.0f;
  const float block_area_height = 2.0f;
  HollowRectangle block_area_outline(model_mat_location, glm::vec2(0.5f, 1.0f),
				     block_area_width / 2.0f, block_area_height / 2.0f,
				     glm::vec3(0.0f, 1.0f, 0.0f));
  root.addDrawable(&block_area_outline);
  NullDrawable block_scale_area(
    glm::scale(glm::mat4(), glm::vec3(1.0f / 12.0f, 1.0f / 12.0f, 1.0f)));
  block_area_outline.addDrawable(&block_scale_area);
  PieceI ipiece(model_mat_location, 0, 12);
  block_scale_area.addDrawable(&ipiece);

  update_event_type = SDL_RegisterEvents(1);
  if(update_event_type == (Uint32)-1) {
    std::cerr << "Unable to register custom update event" << std::endl;
    return 5;
  }

  Uint32 delay = 1000;
  SDL_AddTimer(delay, timer_callback, 0);
  

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    TransformStack ts;
    root.renderHierarchy(ts);

    SDL_GL_SwapWindow(window);
    SDL_Delay(20);
  } while(handle_user_input(&ipiece));

  return 0;
}

bool handle_user_input(ActivePiece* piece) {
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
	piece->moveLeft();
      }
      else if(sdl_event.key.keysym.sym == SDLK_RIGHT) {
	piece->moveRight();
      }
      break;
    }

    default: {
      if(sdl_event.type == update_event_type) {
	piece->moveDown();
      }
      break;
    }
    }
  }


  return !is_quit;
}

Uint32 timer_callback(Uint32 interval, void* param) {
  SDL_Event event;
  SDL_memset(&event, 0, sizeof(event));
  event.type = update_event_type;
  SDL_PushEvent(&event);
  return interval;
}
