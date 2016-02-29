#include "Grid.h"

namespace gui {

Grid::Grid(int w, int h) :
  width(w),
  height(h) {

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

bool Grid::isOutside(int x, int y) const {
  x = abs(x);
  y = abs(y);
  return x >= width / 2 || y >= height / 2;
}

}
