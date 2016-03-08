#include "PieceI.h"
#include "GridSquare.h"

#include <iostream>

namespace gfx {

void PieceI::regen_positions() {
  switch(num_rotations) {
  case 0:
  case 2: {
    componentParts.at(0)->set_position(glm::vec2(x - 1.5f * box_length, y - 0.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x + 1.5f * box_length, y - 0.5f * box_length));
    break;
  }

  case 1:
  case 3: {
    componentParts.at(0)->set_position(glm::vec2(x - 0.5f, y + 1.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f, y + 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x - 0.5f, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x - 0.5f, y - 1.5f * box_length));
    break;
  }

  default: {
    break;
  }
  }
}

PieceI::PieceI(GLuint mul, int _x, int _y) :
  ActivePiece(mul, _x, _y) {
  const glm::vec3 blue(0.0f, 1.0f, 1.0f);
  // Positions are invalid here, but are fixed when regen_positions() called before returning the object
  for(int i = 0; i < 4; ++i) {
    GridSquare* gs = new GridSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue);
    componentParts.push_back(gs);
    addDrawable(gs);
  }

  regen_positions();
}

PieceI::~PieceI() { }

}
