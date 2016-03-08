#ifndef PIECE_I_H
#define PIECE_I_H

#include "ActivePiece.h"

namespace gfx {

class PieceI : public ActivePiece {
protected:
  virtual void regen_positions();

public:
  PieceI(GLuint mul, int _x, int _y);
  virtual ~PieceI();
};

}

#endif
