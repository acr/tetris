#ifndef PIECE_J_H
#define PIECE_J_H

#include "ActivePiece.h"

namespace gfx {

class PieceJ : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceJ(GLuint mul, int _x, int _y);
  virtual ~PieceJ();
};

}

#endif
