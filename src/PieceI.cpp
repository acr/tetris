#include "PieceI.h"

#include <iostream>

namespace gfx {

void PieceI::regen_positions() {
  switch(num_rotations) {
  case 0:
  case 2: {
    componentParts.at(0)->set_position(glm::vec2(x - 1.5f * box_length, y - 0.5f * box_length));
    outlines.at(0)->set_position(glm::vec2(x - 1.5f * box_length, y - 0.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    outlines.at(1)->set_position(glm::vec2(x - 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    outlines.at(2)->set_position(glm::vec2(x + 0.5f * box_length, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x + 1.5f * box_length, y - 0.5f * box_length));
    outlines.at(3)->set_position(glm::vec2(x + 1.5f * box_length, y - 0.5f * box_length));
    break;
  }

  case 1:
  case 3: {
    componentParts.at(0)->set_position(glm::vec2(x - 0.5f, y + 1.5f * box_length));
    outlines.at(0)->set_position(glm::vec2(x - 0.5f, y + 1.5f * box_length));
    componentParts.at(1)->set_position(glm::vec2(x - 0.5f, y + 0.5f * box_length));
    outlines.at(1)->set_position(glm::vec2(x - 0.5f, y + 0.5f * box_length));
    componentParts.at(2)->set_position(glm::vec2(x - 0.5f, y - 0.5f * box_length));
    outlines.at(2)->set_position(glm::vec2(x - 0.5f, y - 0.5f * box_length));
    componentParts.at(3)->set_position(glm::vec2(x - 0.5f, y - 1.5f * box_length));
    outlines.at(3)->set_position(glm::vec2(x - 0.5f, y - 1.5f * box_length));
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
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, blue));

  addDrawable(componentParts.at(0));
  addDrawable(componentParts.at(1));
  addDrawable(componentParts.at(2));
  addDrawable(componentParts.at(3));

  const glm::vec3 white(1.0f, 1.0f, 1.0f);
  // Positions are invalid here, but are fixed when regen_positions() called before returning the object
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(0.0f, 0.0f), box_length / 2.0f, box_length / 2.0f, white));

  addDrawable(outlines.at(0));
  addDrawable(outlines.at(1));
  addDrawable(outlines.at(2));
  addDrawable(outlines.at(3));

  regen_positions();
}

PieceI::~PieceI() {
  for(std::vector<FilledSquare*>::iterator it = componentParts.begin();
      it != componentParts.end(); ++it) {
    delete *it;
  }
  componentParts.clear();

  for(std::vector<HollowRectangle*>::iterator it = outlines.begin();
      it != outlines.end(); ++it) {
    delete *it;
  }
  outlines.clear();
}

void PieceI::moveDown() {
  y--;
  regen_positions();
}

void PieceI::moveUp() {
  y++;
}

void PieceI::rotate() {
  num_rotations = (num_rotations + 1) % 4;
  regen_positions();
}

void PieceI::moveLeft() {
  x--;
  regen_positions();
}

void PieceI::moveRight() {
  x++;
  regen_positions();
}

}
