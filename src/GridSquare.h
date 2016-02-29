#ifndef GRID_SQUARE_H
#define GRID_SQUARE_H

#include "FilledSquare.h"
#include "HollowRectangle.h"

namespace gfx {

class GridSquare : public DrawableHierarchy {
private:
  FilledSquare* square;
  HollowRectangle* rectangle;

protected:
  virtual void renderSelf(const glm::mat4& xfMtx) {}

public:
  GridSquare(GLint mul, const glm::vec2& p, float hw, const glm::vec3& c);
  virtual ~GridSquare();
  void set_position(const glm::vec2& p);
  const glm::vec2& get_centerpoint() const;
};

}

#endif
