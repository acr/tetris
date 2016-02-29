#include "ActivePiece.h"

namespace gfx {

void ActivePiece::renderSelf(const glm::mat4& xfMtx) {}

ActivePiece::~ActivePiece() {};

void ActivePiece::moveSquares(std::vector<GridSquare*>& dest) {
  dest = componentParts;
  componentParts.clear();
}

}
