#ifndef HOLLOW_RECTANGLE_H
#define HOLLOW_RECTANGLE_H

#include "DrawableHierarchy.h"

namespace gfx {

class HollowRectangle : public DrawableHierarchy {
private:
  GLint model_uniform_location;
  GLuint model_vao;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec3 color;
  
  void init_vao(const glm::vec2& lower_left, const glm::vec2& upper_right);

protected:
  virtual void renderSelf(const glm::mat4& xfMtx);

public:
  HollowRectangle(GLint mul, const glm::vec2& center_point, float half_width, float half_height, const glm::vec3& c);
  virtual ~HollowRectangle();
  void render();
};

}

#endif
