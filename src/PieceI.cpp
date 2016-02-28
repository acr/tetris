#include "PieceI.h"

namespace gfx {

void PieceI::regen_xform_mtx() {
  transform_matrix = glm::rotate(
    glm::translate(glm::mat4(), glm::vec3(static_cast<float>(x), static_cast<float>(y), 0.0f)),
    glm::radians(static_cast<float>(num_rotations * M_PI / 2.0)),
    glm::vec3(0.0f, 0.0f, 1.0f));
}

PieceI::PieceI(GLuint mul, int _x, int _y) :
  model_uniform_location(mul),
  x(_x),
  y(_y),
  num_rotations(0) {
  regen_xform_mtx();
  const float box_length = 1.0f;

  const glm::vec3 blue(0.0f, 0.0f, 0.5f);
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(-1.5f * box_length, -0.5f * box_length),
		     box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(-0.5f * box_length, -0.5f * box_length),
		     box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(0.5f * box_length, -0.5f * box_length),
		     box_length / 2.0f, blue));
  componentParts.push_back(
    new FilledSquare(mul, glm::vec2(1.5f * box_length, -0.5f * box_length),
		     box_length / 2.0f, blue));

  addDrawable(componentParts.at(0));
  addDrawable(componentParts.at(1));
  addDrawable(componentParts.at(2));
  addDrawable(componentParts.at(3));

  const glm::vec3 white(1.0f, 1.0f, 1.0f);
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(-1.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(-0.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(0.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));
  outlines.push_back(
    new HollowRectangle(mul, glm::vec2(1.5f * box_length, -0.5f * box_length), box_length / 2.0f, box_length / 2.0f, white));

  addDrawable(outlines.at(0));
  addDrawable(outlines.at(1));
  addDrawable(outlines.at(2));
  addDrawable(outlines.at(3));
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
  regen_xform_mtx();
}

void PieceI::moveUp() {
  y++;
}

void PieceI::rotate() {
  num_rotations = (num_rotations + 1) % 4;
  regen_xform_mtx();
}

void PieceI::moveLeft() {
  x--;
  regen_xform_mtx();
}

void PieceI::moveRight() {
  x++;
  regen_xform_mtx();
}

}
