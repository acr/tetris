#ifndef ACTIVE_PIECE_H
#define ACTIVE_PIECE_H

#include "DrawableHierarchy.h"
#include "FilledSquare.h"
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

  virtual void moveDown()=0;
  virtual void moveUp()=0;
  virtual void rotate()=0;
  virtual void moveLeft()=0;
  virtual void moveRight()=0;
};

}
#endif
