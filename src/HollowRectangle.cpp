#include "HollowRectangle.h"

namespace gfx {

void HollowRectangle::init_vao(const glm::vec2& lower_left, const glm::vec2& upper_right) {
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

void HollowRectangle::renderSelf(const glm::mat4& xfMtx) {
  glUniformMatrix4fv(model_uniform_location, 1, 0, glm::value_ptr(xfMtx));
  dsp::checkGL();
  glBindVertexArray(model_vao);
  dsp::checkGL();
  glDrawArrays(GL_LINES, 0, 8);
  dsp::checkGL();
}

HollowRectangle::HollowRectangle(GLint mul, const glm::vec2& p, float half_width, float half_height, const glm::vec3& c) :
  model_uniform_location(mul),
  color(c),
  center_point(p) {
  transform_matrix = glm::translate(glm::mat4(), glm::vec3(center_point, 0.0f));
  glm::vec2 lower_left(-half_width, -half_height);
  glm::vec2 upper_right(half_width, half_height);
  init_vao(lower_left, upper_right);
}

HollowRectangle::~HollowRectangle() {
  glDeleteBuffers(1, &color_vbo);
  glDeleteBuffers(1, &vertex_vbo);
  glDeleteVertexArrays(1, &model_vao);
}

void HollowRectangle::set_position(const glm::vec2& p) {
  center_point = p;
  transform_matrix = glm::translate(glm::mat4(), glm::vec3(center_point, 0.0f));
}

}
