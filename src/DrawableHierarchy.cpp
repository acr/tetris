#include "DrawableHierarchy.h"

namespace gfx {

DrawableHierarchy::~DrawableHierarchy() {}

void DrawableHierarchy::renderHierarchy(TransformStack& transform_stack) {
  transform_stack.push(transform_matrix);
  for(std::list<DrawableHierarchy*>::iterator it = drawables.begin();
      it != drawables.end(); ++it) {
    (*it)->renderHierarchy(transform_stack);
  }
  renderSelf(transform_stack.getXfMtx());
  transform_stack.pop();
}

void DrawableHierarchy::addDrawable(DrawableHierarchy* drawable) {
  drawables.push_back(drawable);
}

void DrawableHierarchy::removeDrawable(DrawableHierarchy* drawable) {
  for(std::list<DrawableHierarchy*>::iterator it = drawables.begin();
      it != drawables.end(); ++it) {
    if(*it == drawable) {
      drawables.erase(it);
      break;
    }
  }
}

}
