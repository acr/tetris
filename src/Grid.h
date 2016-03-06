#ifndef GRID_H
#define GRID_H

#include "DrawableHierarchy.h"
#include "GridSquare.h"
#include <vector>
#include <set>

namespace gfx {

class ActivePiece;

class Grid : public DrawableHierarchy {
public:
  struct Space {
    gfx::GridSquare* gridSquare;
    int x;
    int y;

    /**
     * Sort in non-decreasing order by height, then by row
     */
    bool operator<(const Space& rhs) const {
      if(y == rhs.y) {
	return x < rhs.x;
      }
      return y < rhs.y;
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
  void getFilledSpaces(std::vector<Space>& dest) const;
  void getFilledSpaces(std::set<Space>& dest) const;
  bool setSpace(gfx::GridSquare* gridSquare, int x, int y);
  void clearSpaceAt(int x, int y);
  gfx::GridSquare* getGridSquareAt(int x, int y) const;
  bool isOutside(const glm::vec2& space) const;
  bool isOutside(int x, int y) const;
  bool addPieceSquaresToGrid(ActivePiece* p);

  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getFirstRow() const { return -height / 2; }
  int getEndRow() const { return height / 2; }
  int getFirstColumn() const { return -width / 2; }
  int getEndColumn() const { return width / 2; }
};

}

#endif
