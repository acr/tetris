#ifndef NULL_DRAWABLE_H
#define NULL_DRAWABLE_H

#include "DrawableHierarchy.h"

namespace gfx {

class NullDrawable : public DrawableHierarchy {
protected:
  virtual void renderSelf(const glm::mat4& xfMtx);

public:
  NullDrawable() {}
  explicit NullDrawable(const glm::mat4& xformMtx);
  virtual ~NullDrawable();
};

}

#endif
