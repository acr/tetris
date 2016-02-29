#include "Grid.h"
#include "ActivePiece.h"
#include <assert.h>

namespace gfx {

Grid::Grid(int w, int h) :
  width(w),
  height(h) {

}

Grid::~Grid() {
  for(std::set<Space>::iterator it = spaces.begin(); it != spaces.end(); ++it) {
    delete it->element;
  }
}

bool Grid::isSpaceOccupied(const glm::vec2& space) const {
  return isSpaceOccupied(roundToSlot(space.x), roundToSlot(space.y));
}

bool Grid::isSpaceOccupied(int xcoord, int ycoord) const {
  Space s;
  s.x = xcoord;
  s.y = ycoord;
  return spaces.find(s) != spaces.end();
}

void Grid::getFilledSpaces(std::vector<Space>& dest) const {
  dest.clear();
  for(std::set<Space>::const_iterator it = spaces.begin();
      it != spaces.end(); ++it) {
    dest.push_back(*it);
  }
}

bool Grid::setSpace(gfx::DrawableHierarchy* element, int x, int y) {
  if(!isSpaceOccupied(x, y)) {
    Space s;
    s.x = x;
    s.y = y;
    s.element = element;
    spaces.insert(s);
    return true;
  }
  return false;
}

void Grid::removeSpace(int x, int y) {
  Space s;
  s.x = x;
  s.y = y;
  spaces.erase(s);
}

gfx::DrawableHierarchy* Grid::getElementAt(int x, int y) const {
  gfx::DrawableHierarchy* rv(0);

  Space s;
  s.x = x;
  s.y = y;
  std::set<Space>::const_iterator it = spaces.find(s);
  if(it != spaces.end()) {
    rv = it->element;
  }

  return rv;
}

bool Grid::isOutside(const glm::vec2& space) const {
  int x = roundToSlot(space.x);
  int y = roundToSlot(space.y);

  bool xinvalid = x >= width / 2;
  if(x < 0) {
    xinvalid = x < -width / 2;
  }

  bool yinvalid = y >= height / 2;
  if(y < 0) {
    yinvalid = y < -height / 2;
  }
  return xinvalid || yinvalid;
}

int Grid::roundToSlot(float coordinate) const {
  if(coordinate < 0) {
    return static_cast<int>(coordinate) - 1;
  }
  return static_cast<int>(coordinate);
}

void Grid::addPieceSquaresToGrid(ActivePiece* p) {
  std::vector<GridSquare*> squares;
  p->moveSquares(squares);
  for(std::vector<GridSquare*>::iterator it = squares.begin();
      it != squares.end(); ++it) {
    const glm::vec2& pos = (*it)->get_centerpoint();
    Space s;
    s.x = roundToSlot(pos.x);
    s.y = roundToSlot(pos.y);
    assert(!isSpaceOccupied(s.x, s.y));
    s.element = *it;
    spaces.insert(s);
    addDrawable(*it);
  }
}

}
