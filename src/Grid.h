#ifndef GRID_H
#define GRID_H

#include "DrawableHierarchy.h"
#include <vector>
#include <set>

namespace gfx {

class ActivePiece;

class Grid : public DrawableHierarchy {
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

protected:
  virtual void renderSelf(const glm::mat4& xfMtx) {}

private:
  std::set<Space> spaces;
  const int width;
  const int height;

  int roundToSlot(float coordinate) const;

public:
  Grid(int w, int h);
  virtual ~Grid();

  bool isSpaceOccupied(const glm::vec2& space) const;
  bool isSpaceOccupied(int x, int y) const;
  void getFilledSpaces(std::vector<Space>& spaces) const;
  bool setSpace(gfx::DrawableHierarchy* element, int x, int y);
  void removeSpace(int x, int y);
  gfx::DrawableHierarchy* getElementAt(int x, int y) const;
  bool isOutside(const glm::vec2& space) const;
  void addPieceSquaresToGrid(ActivePiece* p);
};

}

#endif
