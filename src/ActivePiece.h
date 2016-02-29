#ifndef ACTIVE_PIECE_H
#define ACTIVE_PIECE_H

#include "DrawableHierarchy.h"
#include "FilledSquare.h"
#include "Grid.h"
#include <vector>

namespace gfx {

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

  virtual bool moveDown(const gui::Grid* grid)=0;
  virtual bool moveUp(const gui::Grid* grid)=0;
  virtual bool rotate(const gui::Grid* grid)=0;
  virtual bool moveLeft(const gui::Grid* grid)=0;
  virtual bool moveRight(const gui::Grid* grid)=0;
};

}
#endif
