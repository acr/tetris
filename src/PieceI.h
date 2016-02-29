#ifndef PIECE_I_H
#define PIECE_I_H

#include "ActivePiece.h"
#include "HollowRectangle.h"

namespace gfx {

class PieceI : public ActivePiece {
private:
  GLuint model_uniform_location;
  int x;
  int y;
  int num_rotations;
  const float box_length;
  std::vector<HollowRectangle*> outlines;

  void regen_positions();
  bool is_collision(const gui::Grid* grid) const;

public:
  PieceI(GLuint mul, int _x, int _y);
  virtual ~PieceI();

  virtual bool moveDown(const gui::Grid* grid);
  virtual bool moveUp(const gui::Grid* grid);
  virtual bool rotate(const gui::Grid* grid);
  virtual bool moveLeft(const gui::Grid* grid);
  virtual bool moveRight(const gui::Grid* grid);
};

}

#endif
