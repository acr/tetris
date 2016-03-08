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

static const int width = 256;
static const int height = 512;

using namespace gfx;

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

  // Enable transparancy via blending
  glEnable(GL_BLEND);
  dsp::checkGL();
  // This calculates a pixel color channel as follows:
  // C:  The blend result for one color channel of a pixel
  // M:  The maximum value for this channel
  // c1: The value of the channel in the first source
  // s1: The scale factor for the first source
  // c2: The value of the channel in the second source
  // s2: The scale factor for the second source
  // C = min(M, c1 * s1 + c2 * s2)
  // If source alpha is A, these arguments set s1 and s2 to:
  // A, (1 - A), respectively
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  dsp::checkGL();

  // Disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  dsp::checkGL();

  gui::GameUI ui(window, width, height);
  ui.run();

  SDL_GL_DeleteContext(glContext);
  SDL_Quit();

  return 0;
}
