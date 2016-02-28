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
  std::vector<HollowRectangle*> outlines;

  void regen_xform_mtx();
public:
  PieceI(GLuint mul, int _x, int _y);
  virtual ~PieceI();
  virtual void moveDown();
  virtual void moveUp();
  virtual void rotate();
  virtual void moveLeft();
  virtual void moveRight();
};

}

#endif
