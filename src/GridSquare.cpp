#include "GridSquare.h"

namespace gfx {

GridSquare::GridSquare(GLint mul, const glm::vec2& p, float hw, const glm::vec3& c) {
  square = new FilledSquare(mul, p, hw, c);
  
  const glm::vec3 white(1.0f, 1.0f, 1.0f);
  rectangle = new HollowRectangle(mul, p, hw, hw, white);

  addDrawable(square);
  addDrawable(rectangle);
}

GridSquare::~GridSquare() {
  delete square;
  delete rectangle;
}

void GridSquare::set_position(const glm::vec2& p) {
  square->set_position(p);
  rectangle->set_position(p);
}

const glm::vec2& GridSquare::get_centerpoint() const {
  return square->get_centerpoint();
}

}
