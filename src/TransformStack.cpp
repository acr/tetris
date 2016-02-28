#include "TransformStack.h"

namespace gfx {

TransformStack::TransformStack() : isDirty(false) {}

void TransformStack::push(const glm::mat4& mtx) {
  xfStack.push_back(mtx);
  isDirty = true;
}

glm::mat4 TransformStack::pop() {
  glm::mat4 rv;
  if(!xfStack.empty()) {
    rv = xfStack.back();
    xfStack.pop_back();
  }
  isDirty = true;
  return rv;
}    

const glm::mat4& TransformStack::getXfMtx() {
  if(isDirty) {
    xfMtx = glm::mat4();
    for(std::vector<glm::mat4>::const_iterator it = xfStack.begin();
	it != xfStack.end(); ++it) {
      xfMtx *= *it;
    }

    isDirty = false;
  }

  return xfMtx;
}

}
