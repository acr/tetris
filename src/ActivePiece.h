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

  virtual void renderSelf(const glm::mat4& xfMtx);

public:
  virtual ~ActivePiece();

  void moveSquares(std::vector<GridSquare*>& dest);
  virtual bool moveDown(const Grid* grid)=0;
  virtual bool moveUp(const Grid* grid)=0;
  virtual bool rotate(const Grid* grid)=0;
  virtual bool moveLeft(const Grid* grid)=0;
  virtual bool moveRight(const Grid* grid)=0;
};

}
#endif
