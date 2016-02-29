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
  model_uniform_location(mul),
  x(_x),
  y(_y),
  num_rotations(0),
  box_length(1.0f) {

  const glm::vec3 blue(0.0f, 0.0f, 0.5f);
  // Positions are invalid here, but are fixed when regen_positions() called before returning the object
  for(int i = 0; i < 4; ++i) {
    GridSquare* gs = new GridSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue);
    componentParts.push_back(gs);
    addDrawable(gs);
  }

  regen_positions();
}

PieceI::~PieceI() {
  for(std::vector<GridSquare*>::iterator it = componentParts.begin();
      it != componentParts.end(); ++it) {
    delete *it;
  }
  componentParts.clear();
}

bool PieceI::moveDown(const Grid* grid) {
  y--;
  regen_positions();
  if(is_collision(grid)) {
    y++;
    regen_positions();
    return false;
  }
  return true;
}

bool PieceI::moveUp(const Grid* grid) {
  y++;
  regen_positions();
  if(is_collision(grid)) {
    y--;
    regen_positions();
    return false;
  }
  return true;
}

bool PieceI::rotate(const Grid* grid) {
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

bool PieceI::moveLeft(const Grid* grid) {
  x--;
  regen_positions();
  if(is_collision(grid)) {
    x++;
    regen_positions();
    return false;
  }
  return true;
}

bool PieceI::moveRight(const Grid* grid) {
  x++;
  regen_positions();
  if(is_collision(grid)) {
    x--;
    regen_positions();
    return false;
  }
  return true;
}

bool PieceI::is_collision(const Grid* grid) const {
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
