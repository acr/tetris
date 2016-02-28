#include "DrawableHierarchy.h"

namespace gfx {

DrawableHierarchy::~DrawableHierarchy() {}

void DrawableHierarchy::renderHierarchy(TransformStack& transform_stack) {
  transform_stack.push(transform_matrix);
  for(std::vector<DrawableHierarchy*>::iterator it = drawables.begin();
      it != drawables.end(); ++it) {
    (*it)->renderHierarchy(transform_stack);
  }
  renderSelf(transform_stack.getXfMtx());
  transform_stack.pop();
}

void DrawableHierarchy::addDrawable(DrawableHierarchy* drawable) {
  drawables.push_back(drawable);
}


}
