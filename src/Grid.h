#ifndef GRID_H
#define GRID_H

#include "DrawableHierarchy.h"
#include <vector>
#include <set>

namespace gui {

class Grid {
public:
  struct Space {
    gfx::DrawableHierarchy* element;
    int x;
    int y;

    bool operator<(const Space& rhs) const {
      if(x == rhs.x) {
	return y < rhs.y;
      }
      return x < rhs.x;
    }
  };

private:
  std::set<Space> spaces;
  const int width;
  const int height;

public:
  Grid(int w, int h);

  bool isSpaceOccupied(int xcoord, int ycoord) const;
  void getFilledSpaces(std::vector<Space>& spaces) const;
  bool setSpace(gfx::DrawableHierarchy* element, int x, int y);
  void removeSpace(int x, int y);
  gfx::DrawableHierarchy* getElementAt(int x, int y) const;
  bool isOutside(int x, int y) const;
};

}

#endif
