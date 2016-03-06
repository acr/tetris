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
    delete it->gridSquare;
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

void Grid::getFilledSpaces(std::set<Space>& dest) const {
  dest = spaces;
}

bool Grid::setSpace(gfx::GridSquare* gridSquare, int x, int y) {
  if(!isSpaceOccupied(x, y)) {
    Space s;
    s.x = x;
    s.y = y;
    s.gridSquare = gridSquare;
    gridSquare->set_position(glm::vec2(x + 0.5f, y + 0.5f));
    spaces.insert(s);
    addDrawable(gridSquare);
    return true;
  }
  return false;
}

void Grid::clearSpaceAt(int x, int y) {
  if(!isSpaceOccupied(x, y)) {
    return;
  }

  gfx::GridSquare* gridSquare = getGridSquareAt(x, y);
  removeDrawable(gridSquare);

  Space s;
  s.x = x;
  s.y = y;
  spaces.erase(s);
}

gfx::GridSquare* Grid::getGridSquareAt(int x, int y) const {
  gfx::GridSquare* rv(0);

  Space s;
  s.x = x;
  s.y = y;
  std::set<Space>::const_iterator it = spaces.find(s);
  if(it != spaces.end()) {
    rv = it->gridSquare;
  }

  return rv;
}

bool Grid::isOutside(const glm::vec2& space) const {
  return isOutside(roundToSlot(space.x), roundToSlot(space.y));
}

bool Grid::isOutside(int x, int y) const {
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
    int x = roundToSlot(pos.x);
    int y = roundToSlot(pos.y);
    assert(!isSpaceOccupied(x, y));
    setSpace(*it, x, y);
  }
}

}
