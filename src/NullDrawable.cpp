#include "NullDrawable.h"

namespace gfx {

NullDrawable::NullDrawable(const glm::mat4& xformMtx) {
    transform_matrix = xformMtx;
}

NullDrawable::~NullDrawable() { }

void NullDrawable::renderSelf(const glm::mat4& xfMtx) { }

}
