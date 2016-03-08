#ifndef ACTIVE_PIECE_H
#define ACTIVE_PIECE_H

#include "DrawableHierarchy.h"
#include "GridSquare.h"
#include "Grid.h"
#include <vector>

namespace gfx {

class ActivePiece : public DrawableHierarchy {
protected:
  std::vector<GridSquare*> componentParts;
  GLuint model_uniform_location;
  int x;
  int y;
  int num_rotations;
  const float box_length;

  virtual void renderSelf(const glm::mat4& xfMtx);
  bool is_collision(const Grid* grid) const;
  ActivePiece(GLuint mul, int _x, int _y);
  virtual void regen_positions()=0;

public:
  virtual ~ActivePiece();

  void transferSquaresTo(std::vector<GridSquare*>& dest);
  virtual bool moveDown(const Grid* grid);
  virtual bool moveUp(const Grid* grid);
  virtual bool rotate(const Grid* grid);
  virtual bool moveLeft(const Grid* grid);
  virtual bool moveRight(const Grid* grid);
};

}
#endif
