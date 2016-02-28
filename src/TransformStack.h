#include "gl_defns.h"
#include <vector>

#ifndef TRANSFORM_STACK_H
#define TRANSFORM_STACK_H

namespace gfx {

class TransformStack {
private:
  std::vector<glm::mat4> xfStack;
  glm::mat4 xfMtx;
  bool isDirty;

public:
  TransformStack();
  void push(const glm::mat4& mtx);
  glm::mat4 pop();
  const glm::mat4& getXfMtx();
};

}

#endif
