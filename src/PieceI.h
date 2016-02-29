#ifndef PIECE_I_H
#define PIECE_I_H

#include "ActivePiece.h"

namespace gfx {

class PieceI : public ActivePiece {
private:
  GLuint model_uniform_location;
  int x;
  int y;
  int num_rotations;
  const float box_length;

  void regen_positions();
  bool is_collision(const Grid* grid) const;

public:
  PieceI(GLuint mul, int _x, int _y);
  virtual ~PieceI();

  virtual bool moveDown(const Grid* grid);
  virtual bool moveUp(const Grid* grid);
  virtual bool rotate(const Grid* grid);
  virtual bool moveLeft(const Grid* grid);
  virtual bool moveRight(const Grid* grid);
};

}

#endif
