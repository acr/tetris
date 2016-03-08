#include "PieceJ.h"
#include "GridSquare.h"

namespace gfx {

void PieceJ::regen_positions() {
  switch(num_rotations) {
  case 0: {
    componentParts.at(0)->set_position(glm::vec2(x - 1.5f * box_length, y + 0.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f * box_length, y + 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x + 0.5f * box_length, y + 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    break;
  }

  case 1: {
    componentParts.at(0)->set_position(glm::vec2(x + 0.5f * box_length, y + 1.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x + 0.5f * box_length, y + 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    break;
  }

  case 2: {
    componentParts.at(0)->set_position(glm::vec2(x + 1.5f * box_length, y - 0.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x - 0.5f * box_length, y + 0.5f * box_length));
    break;
  }

  case 3: {
    componentParts.at(0)->set_position(glm::vec2(x - 0.5f * box_length, y - 1.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x - 0.5f * box_length, y + 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x + 0.5f * box_length, y + 0.5f * box_length));
    break;
  }

  default: {
    break;
  }
  }
}

PieceJ::PieceJ(GLuint mul, int _x, int _y) :
  ActivePiece(mul, _x, _y, glm::vec3(0.0f, 0.0f, 1.0f)) {

  regen_positions();
}

PieceJ::~PieceJ() { }

}
