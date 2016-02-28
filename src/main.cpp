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
#include "GameUI.h"
#include <iostream>
#include <math.h>
#include <vector>

static const int width = 512;
static const int height = 1024;

using namespace gfx;

class PLC : public dsp::PreLinkCallback {
public:
  virtual ~PLC() {}
  virtual void preLinkCallback(GLint shader_program) { }
};

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

  gui::GameUI ui(window, model_mat_location, width, height);
  ui.run();

  SDL_GL_DeleteContext(glContext);
  SDL_Quit();

  return 0;
}
