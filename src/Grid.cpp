#include "Grid.h"
#include "ActivePiece.h"
#include <assert.h>
#include <string.h>

namespace gfx {

Grid::Grid(int w, int h) :
  width(w),
  height(h) {

  grid = new Space*[width * height];
  memset(grid, 0, sizeof(Space*) * width * height);
}

Grid::~Grid() {
  for(int i = 0; i < width * height; i++) {
    if(grid[i]) {
      if(grid[i]->gridSquare != 0) {
	delete grid[i]->gridSquare;
      }
      delete grid[i];
    }
  }
  delete [] grid;
}

bool Grid::isSpaceOccupied(const glm::vec2& space) const {
  return isSpaceOccupied(roundToSlot(space.x), roundToSlot(space.y));
}

bool Grid::isSpaceOccupied(int xcoord, int ycoord) const {
  ycoord += height / 2;
  xcoord += width / 2;
  return grid[ycoord * width + xcoord] != 0;
}

void Grid::getFilledSpaces(std::unordered_set<Space*>& dest) const {
  dest = filledSpaces;
}

bool Grid::setSpace(gfx::GridSquare* gridSquare, int x, int y) {
  if(!isSpaceOccupied(x, y) && !isOutside(x, y)) {
    gridSquare->set_position(glm::vec2(x + 0.5f, y + 0.5f));
    Space* s = new Space;
    s->gridSquare = gridSquare;
    s->x = x;
    s->y = y;

    y += height / 2;
    x += width / 2;
    grid[y * width + x] = s;
    addDrawable(gridSquare);
    filledSpaces.insert(s);
    return true;
  }
  return false;
}

void Grid::clearSpaceAt(int x, int y) {
  if(!isSpaceOccupied(x, y)) {
    return;
  }

  y += height / 2;
  x += width / 2;
  Space* s = grid[y * width + x];
  filledSpaces.erase(s);
  gfx::GridSquare* gridSquare = s->gridSquare;
  delete s;
  grid[y * width + x] = 0;
  removeDrawable(gridSquare);
}

Grid::Space* Grid::getSpaceAt(int x, int y) const {
  if(!isSpaceOccupied(x, y)) {
    return 0;
  }

  y += height / 2;
  x += width / 2;
  return grid[y * width + x];
}

bool Grid::isOutside(const glm::vec2& space) const {
  return isOutside(roundToSlot(space.x), roundToSlot(space.y));
}

bool Grid::isOutside(int x, int y) const {
  y += height / 2;
  x += width / 2;
  return x < 0 || y < 0 || x >= width || y >= height;
}

int Grid::roundToSlot(float coordinate) const {
  if(coordinate < 0) {
    return static_cast<int>(coordinate) - 1;
  }
  return static_cast<int>(coordinate);
}

bool Grid::addPieceSquaresToGrid(ActivePiece* p) {
  bool returnValue = true;
  std::vector<GridSquare*> squares;
  p->transferSquaresTo(squares);
  for(std::vector<GridSquare*>::iterator it = squares.begin();
      it != squares.end(); ++it) {
    const glm::vec2& pos = (*it)->get_centerpoint();
    int x = roundToSlot(pos.x);
    int y = roundToSlot(pos.y);
    if(!setSpace(*it, x, y)) {
      returnValue = false;
    }
  }
  return returnValue;
}

}
