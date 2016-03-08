#ifndef PIECE_Z_H
#define PIECE_Z_H

#include "ActivePiece.h"

namespace gfx {

class PieceZ : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceZ(GLuint mul, int _x, int _y);
  virtual ~PieceZ();
};

}

#endif
