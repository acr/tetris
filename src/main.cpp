#include "gl_defns.h"
#include "gl_utils.h"

#include <iostream>
#include <math.h>
#include <vector>

static const int width = 512;
static const int height = 1024;
static Uint32 update_event_type = 0;

class PLC : public dsp::PreLinkCallback {
public:
  virtual ~PLC() {}
  virtual void preLinkCallback(GLint shader_program) { }
};

class TransformStack {
private:
  std::vector<glm::mat4> xfStack;
  glm::mat4 xfMtx;
  bool isDirty;

public:
  TransformStack() : isDirty(false) {}

  void push(const glm::mat4& mtx) {
    xfStack.push_back(mtx);
    isDirty = true;
  }

  glm::mat4 pop() {
    glm::mat4 rv;
    if(!xfStack.empty()) {
      rv = xfStack.back();
      xfStack.pop_back();
    }
    isDirty = true;
    return rv;
  }    

  const glm::mat4& getXfMtx() {
    if(isDirty) {
      xfMtx = glm::mat4();
      for(std::vector<glm::mat4>::const_iterator it = xfStack.begin();
	  it != xfStack.end(); ++it) {
	xfMtx *= *it;
      }

      isDirty = false;
    }

    return xfMtx;
  }
};

class DrawableHierarchy {
protected:
  std::vector<DrawableHierarchy*> drawables;
  glm::mat4 transform_matrix;
  virtual void renderSelf(const glm::mat4& xfMtx)=0;

public:
  virtual ~DrawableHierarchy() {}

  void renderHierarchy(TransformStack& transform_stack) {
    transform_stack.push(transform_matrix);
    for(std::vector<DrawableHierarchy*>::iterator it = drawables.begin();
	it != drawables.end(); ++it) {
      (*it)->renderHierarchy(transform_stack);
    }
    renderSelf(transform_stack.getXfMtx());
    transform_stack.pop();
  }

  void addDrawable(DrawableHierarchy* drawable) {
    drawables.push_back(drawable);
  }
};

class NullDrawable : public DrawableHierarchy {
protected:
  virtual void renderSelf(const glm::mat4& xfMtx) { }

public:
  NullDrawable() {}
  explicit NullDrawable(const glm::mat4& xformMtx) {
    transform_matrix = xformMtx;
  }

  virtual ~NullDrawable() {}
};

class HollowRectangle : public DrawableHierarchy {
private:
  GLint model_uniform_location;
  GLuint model_vao;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec3 color;
  
  void init_vao(const glm::vec2& lower_left, const glm::vec2& upper_right) {
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

protected:
  virtual void renderSelf(const glm::mat4& xfMtx) {
    glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(xfMtx));
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glDrawArrays(GL_LINES, 0, 8);
    dsp::checkGL();
  }

public:
  HollowRectangle(GLint mul, const glm::vec2& center_point, float half_width, float half_height, const glm::vec3& c) :
    model_uniform_location(mul), color(c) {
    transform_matrix = glm::translate(glm::mat4(), glm::vec3(center_point, 0.0f));
    glm::vec2 lower_left(-half_width, -half_height);
    glm::vec2 upper_right(half_width, half_height);
    init_vao(lower_left, upper_right);
  }

  virtual ~HollowRectangle() {}

  void render() {
    glm::mat4 model_mat;
    glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(model_mat));
  }

};

class FilledSquare : public DrawableHierarchy {
private:
  GLuint model_vao;
  GLint model_uniform_location;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec3 color;
  glm::vec2 center_point;
  float half_width;

  void init_vao() {
    glGenBuffers(1, &vertex_vbo);
    dsp::checkGL();
    glGenBuffers(1, &color_vbo);
    dsp::checkGL();
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    dsp::checkGL();

    float triangles[] = {
      -half_width, -half_width,
      half_width, half_width,
      -half_width, half_width,

      -half_width, -half_width,
      half_width, -half_width,
      half_width, half_width,
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

protected:
  virtual void renderSelf(const glm::mat4& xfMtx) {
    glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(xfMtx));
    dsp::checkGL();
    glBindVertexArray(model_vao);
    dsp::checkGL();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    dsp::checkGL();
  }

public:
  FilledSquare(GLint mul, const glm::vec2& p, float hw, const glm::vec3& c) :
    model_uniform_location(mul), color(c), center_point(p), half_width(hw) {
    init_vao();
    transform_matrix = glm::translate(glm::mat4(), glm::vec3(p, 0.0f));
  }

  virtual ~FilledSquare() { }

  bool isIntersectingWith(const FilledSquare& otherShape) const {
    const float centerpoint_distance = glm::length(center_point - otherShape.center_point);
    const float combined_halfwidth = half_width + otherShape.half_width;
    return centerpoint_distance < combined_halfwidth;
  }
};

class ActivePiece : public DrawableHierarchy {
protected:
  std::vector<FilledSquare*> componentParts;

  virtual void renderSelf(const glm::mat4& xfMtx) {}

public:
  virtual ~ActivePiece() {};

  bool isIntersectingWith(const FilledSquare& otherSquare) {
    for(std::vector<FilledSquare*>::const_iterator it = componentParts.begin();
	it != componentParts.end(); ++it) {
      if((*it)->isIntersectingWith(otherSquare)) {
	return true;
      }
    }
    return false;
  }

  virtual void moveDown()=0;
  virtual void moveUp()=0;
  virtual void rotate()=0;
  virtual void moveLeft()=0;
  virtual void moveRight()=0;
};

class PieceI : public ActivePiece {
private:
  GLuint model_uniform_location;
  int x;
  int y;
  int num_rotations;
  std::vector<HollowRectangle*> outlines;

  void regen_xform_mtx() {
    transform_matrix = glm::rotate(
      glm::translate(glm::mat4(), glm::vec3(static_cast<float>(x), static_cast<float>(y), 0.0f)),
      glm::radians(static_cast<float>(num_rotations * M_PI / 2.0)),
      glm::vec3(0.0f, 0.0f, 1.0f));
  }

public:
  PieceI(GLuint mul, int _x, int _y) :
    model_uniform_location(mul),
    x(_x),
    y(_y),
    num_rotations(0) {
    regen_xform_mtx();
    const float box_length = 1.0f;

    const glm::vec3 blue(0.0f, 0.0f, 0.5f);
    componentParts.push_back(
      new FilledSquare(mul, glm::vec2(-1.5f * box_length, -0.5f * box_length),
		       box_length / 2.0f, blue));
    componentParts.push_back(
      new FilledSquare(mul, glm::vec2(-0.5f * box_length, -0.5f * box_length),
		       box_length / 2.0f, blue));
    componentParts.push_back(
      new FilledSquare(mul, glm::vec2(0.5f * box_length, -0.5f * box_length),
		       box_length / 2.0f, blue));
    componentParts.push_back(
      new FilledSquare(mul, glm::vec2(1.5f * box_length, -0.5f * box_length),
		       box_length / 2.0f, blue));

    addDrawable(componentParts.at(0));
    addDrawable(componentParts.at(1));
    addDrawable(componentParts.at(2));
    addDrawable(componentParts.at(3));

    const glm::vec3 white(1.0f, 1.0f, 1.0f);
    outlines.push_back(
      new HollowRectangle(mul, glm::vec2(-1.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
    outlines.push_back(
      new HollowRectangle(mul, glm::vec2(-0.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
    outlines.push_back(
      new HollowRectangle(mul, glm::vec2(0.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
    outlines.push_back(
      new HollowRectangle(mul, glm::vec2(1.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));

    addDrawable(outlines.at(0));
    addDrawable(outlines.at(1));
    addDrawable(outlines.at(2));
    addDrawable(outlines.at(3));
  }

  virtual ~PieceI() {
    for(std::vector<FilledSquare*>::iterator it = componentParts.begin();
	it != componentParts.end(); ++it) {
      delete *it;
    }
    componentParts.clear();

    for(std::vector<HollowRectangle*>::iterator it = outlines.begin();
	it != outlines.end(); ++it) {
      delete *it;
    }
    outlines.clear();
  }

  virtual void moveDown() {
    y--;
    regen_xform_mtx();
  }

  virtual void moveUp() {
    y++;
  }

  virtual void rotate() {
    num_rotations = (num_rotations + 1) % 4;
    regen_xform_mtx();
  }

  virtual void moveLeft() {
    x--;
    regen_xform_mtx();
  }

  virtual void moveRight() {
    x++;
    regen_xform_mtx();
  }
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
