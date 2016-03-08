#include "ActivePiece.h"

namespace gfx {

ActivePiece::ActivePiece(GLuint mul, int _x, int _y, const glm::vec3& color) :
  model_uniform_location(mul),
  x(_x),
  y(_y),
  num_rotations(0),
  box_length(1.0f) {

  // Positions are invalid here, but are fixed when regen_positions() called before returning the object
  for(int i = 0; i < 4; ++i) {
    GridSquare* gs = new GridSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, color);
    componentParts.push_back(gs);
    addDrawable(gs);
  }
}

void ActivePiece::renderSelf(const glm::mat4& xfMtx) {}

ActivePiece::~ActivePiece() {
  for(std::vector<GridSquare*>::iterator it = componentParts.begin();
      it != componentParts.end(); ++it) {
    delete *it;
  }
  componentParts.clear();
};

void ActivePiece::transferSquaresTo(std::vector<GridSquare*>& dest) {
  dest = componentParts;
  componentParts.clear();
}

bool ActivePiece::moveDown(const Grid* grid) {
  y--;
  regen_positions();
  if(is_collision(grid)) {
    y++;
    regen_positions();
    return false;
  }
  return true;
}

bool ActivePiece::moveUp(const Grid* grid) {
  y++;
  regen_positions();
  if(is_collision(grid)) {
    y--;
    regen_positions();
    return false;
  }
  return true;
}

bool ActivePiece::rotate(const Grid* grid) {
  const int old_num_rotations = num_rotations;
  num_rotations = (num_rotations + 1) % 4;
  regen_positions();
  if(is_collision(grid)) {
    num_rotations = old_num_rotations;
    regen_positions();
    return false;
  }
  return true;
}

bool ActivePiece::moveLeft(const Grid* grid) {
  x--;
  regen_positions();
  if(is_collision(grid)) {
    x++;
    regen_positions();
    return false;
  }
  return true;
}

bool ActivePiece::moveRight(const Grid* grid) {
  x++;
  regen_positions();
  if(is_collision(grid)) {
    x--;
    regen_positions();
    return false;
  }
  return true;
}

bool ActivePiece::is_collision(const Grid* grid) const {
  bool is_collision = false;
  for(std::vector<GridSquare*>::const_iterator it = componentParts.begin();
      it != componentParts.end(); ++it) {
    const glm::vec2& center_point = (*it)->get_centerpoint();
    if(grid->isSpaceOccupied(center_point)) {
      is_collision = true;
      break;
    }
    else if(grid->isOutside(center_point)) {
      is_collision = true;
      break;
    }
  }
  return is_collision;
}

}
