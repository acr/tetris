#ifndef DRAWABLE_HIERARCHY_H
#define DRAWABLE_HIERARCHY_H

#include "gl_defns.h"
#include "gl_utils.h"
#include <list>
#include "TransformStack.h"

namespace gfx {

class DrawableHierarchy {
protected:
  std::list<DrawableHierarchy*> drawables;
  glm::mat4 transform_matrix;
  virtual void renderSelf(const glm::mat4& xfMtx)=0;

public:
  virtual ~DrawableHierarchy();
  void renderHierarchy(TransformStack& transform_stack);
  void addDrawable(DrawableHierarchy* drawable);
  void removeDrawable(DrawableHierarchy* drawable);
};

}
#endif
