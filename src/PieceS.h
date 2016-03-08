#ifndef PIECE_S_H
#define PIECE_S_H

#include "ActivePiece.h"

namespace gfx {

class PieceS : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceS(GLuint mul, int _x, int _y);
  virtual ~PieceS();
};

}

#endif
