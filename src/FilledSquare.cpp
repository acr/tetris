#include "FilledSquare.h"

namespace gfx {

void FilledSquare::init_vao() {
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

void FilledSquare::renderSelf(const glm::mat4& xfMtx) {
  glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(xfMtx));
  dsp::checkGL();
  glBindVertexArray(model_vao);
  dsp::checkGL();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  dsp::checkGL();
}

FilledSquare::FilledSquare(GLint mul, const glm::vec2& p, float hw, const glm::vec3& c) :
  model_uniform_location(mul), color(c), center_point(p), half_width(hw) {
  init_vao();
  transform_matrix = glm::translate(glm::mat4(), glm::vec3(center_point, 0.0f));
}

FilledSquare::~FilledSquare() {}

void FilledSquare::set_position(const glm::vec2& p) {
  center_point = p;
  transform_matrix = glm::translate(glm::mat4(), glm::vec3(center_point, 0.0f));
}    

const glm::vec2& FilledSquare::get_centerpoint() const {
  return center_point;
}

}
