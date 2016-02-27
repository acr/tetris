#include "gl_defns.h"
#include "gl_utils.h"

#include <iostream>

static int width = 800;
static int height = 600;

class PLC : public dsp::PreLinkCallback {
public:
  virtual ~PLC() {}
  virtual void preLinkCallback(GLint shader_program) { }
};

bool handle_user_input();

int main() {
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
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
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

  glEnable(GL_DEPTH_TEST);
  dsp::checkGL();
  glDepthFunc(GL_LESS);
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
  GLint proj_mat_location = glGetUniformLocation(shader_program, "P");
  dsp::checkGL();
  GLint view_mat_location = glGetUniformLocation(shader_program, "V");
  dsp::checkGL();
  GLint model_mat_location = glGetUniformLocation(shader_program, "M");
  dsp::checkGL();
  glUseProgram(shader_program);
  glUniformMatrix4fv(proj_mat_location, 1, 0, glm::value_ptr(ident_mat));
  dsp::checkGL();
  glUniformMatrix4fv(view_mat_location, 1, 0, glm::value_ptr(ident_mat));
  dsp::checkGL();
  glUniformMatrix4fv(model_mat_location, 1, 0, glm::value_ptr(ident_mat));
  dsp::checkGL();
  dsp::printAll(shader_program);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    SDL_GL_SwapWindow(window);
    SDL_Delay(20);
  } while(handle_user_input());

  return 0;
}

bool handle_user_input() {
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

    default: {
      break;
    }

    }
  }

  return !is_quit;
}
