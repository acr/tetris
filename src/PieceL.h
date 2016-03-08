#ifndef PIECE_L_H
#define PIECE_L_H

#include "ActivePiece.h"

namespace gfx {

class PieceL : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceL(GLuint mul, int _x, int _y);
  virtual ~PieceL();
};

}

#endif
