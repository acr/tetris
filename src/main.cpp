#include "gl_defns.h"
#include "gl_utils.h"

#include <iostream>
#include <math.h>

static int width = 512;
static int height = 1024;

bool handle_user_input();

class PLC : public dsp::PreLinkCallback {
public:
  virtual ~PLC() {}
  virtual void preLinkCallback(GLint shader_program) { }
};

class Square {
private:
  GLuint model_vao;
  GLint model_uniform_location;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec2 position;
  glm::vec3 color;

  void init_vao() {
    glGenBuffers(1, &vertex_vbo);
    dsp::checkGL();
    glGenBuffers(1, &color_vbo);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    dsp::checkGL();

    float triangles[] = {
      -0.5f, -0.5f,
      0.5f, 0.5f,
      -0.5f, 0.5f,

      -0.5f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    dsp::checkGL();

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    dsp::checkGL();
    
    float colors[] = {
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,

      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
    };      
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    dsp::checkGL();

    glGenVertexArrays(1, &model_vao);
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glEnableVertexAttribArray(0);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    dsp::checkGL();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    dsp::checkGL();
    glEnableVertexAttribArray(1);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    dsp::checkGL();
  }

public:
  explicit Square(GLint mul) : model_uniform_location(mul) { init_vao(); }
  Square(GLint mul, const glm::vec2& p, const glm::vec3& c) :
    model_uniform_location(mul), position(p), color(c) { init_vao(); }

  void render() {
    glm::mat4 model_mat = glm::translate(glm::mat4(), glm::vec3(position, 0.0f));
    glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(model_mat));
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    dsp::checkGL();
  }
};

class Rectangle {
  GLint model_uniform_location;
  GLuint model_vao;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec2 lower_left;
  glm::vec2 upper_right;
  glm::vec3 color;
  
  void init_vao() {
    glGenBuffers(1, &vertex_vbo);
    dsp::checkGL();
    glGenBuffers(1, &color_vbo);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    dsp::checkGL();

    float triangles[] = {
      lower_left.x, lower_left.y,
      upper_right.x, lower_left.y,

      upper_right.x, lower_left.y,
      upper_right.x, upper_right.y,

      upper_right.x, upper_right.y,
      lower_left.x, upper_right.y,

      lower_left.x, upper_right.y,
      lower_left.x, lower_left.y
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    dsp::checkGL();

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    dsp::checkGL();

    float colors[] = {
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b,
      color.r, color.g, color.b
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    dsp::checkGL();

    glGenVertexArrays(1, &model_vao);
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glEnableVertexAttribArray(0);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    dsp::checkGL();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    dsp::checkGL();
    glEnableVertexAttribArray(1);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    dsp::checkGL();
  }

public:
  Rectangle(GLint mul, const glm::vec2& ll, const glm::vec2& ur, const glm::vec3& c) :
    model_uniform_location(mul), lower_left(ll), upper_right(ur), color(c) { init_vao(); }

  void render() {
    glm::mat4 model_mat;
    glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(model_mat));
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glDrawArrays(GL_LINES, 0, 8);
    dsp::checkGL();
  }

};

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


  // Square s(model_mat_location, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  Rectangle r(model_mat_location, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // s.render();
    r.render();

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
