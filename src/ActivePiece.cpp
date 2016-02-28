#include "ActivePiece.h"

namespace gfx {

void ActivePiece::renderSelf(const glm::mat4& xfMtx) {}

ActivePiece::~ActivePiece() {};

bool ActivePiece::isIntersectingWith(const FilledSquare& otherSquare) {
  for(std::vector<FilledSquare*>::const_iterator it = componentParts.begin();
      it != componentParts.end(); ++it) {
    if((*it)->isIntersectingWith(otherSquare)) {
      return true;
    }
  }
  return false;
}

}
