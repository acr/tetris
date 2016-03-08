#ifndef PIECE_O_H
#define PIECE_O_H

#include "ActivePiece.h"

namespace gfx {

class PieceO : public ActivePiece {
private:
  virtual void regen_positions();

public:
  PieceO(GLuint mul, int _x, int _y);
  virtual ~PieceO();
};

}

#endif
