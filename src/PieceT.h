#ifndef PIECE_T_H
#define PIECE_T_H

#include "ActivePiece.h"

namespace gfx {

class PieceT : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceT(GLuint mul, int _x, int _y);
  virtual ~PieceT();
};

}

#endif
