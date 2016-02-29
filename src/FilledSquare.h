#ifndef FILLED_SQUARE_H
#define FILLED_SQUARE_H

#include "DrawableHierarchy.h"

namespace gfx {

class FilledSquare : public DrawableHierarchy {
private:
  GLuint model_vao;
  GLint model_uniform_location;
  GLuint vertex_vbo;
  GLuint color_vbo;
  glm::vec3 color;
  glm::vec2 center_point;
  float half_width;

  void init_vao();

protected:
  virtual void renderSelf(const glm::mat4& xfMtx);

public:
  FilledSquare(GLint mul, const glm::vec2& p, float hw, const glm::vec3& c);
  virtual ~FilledSquare();
  void set_position(const glm::vec2& p);
  const glm::vec2& get_centerpoint() const;
};

}

#endif
