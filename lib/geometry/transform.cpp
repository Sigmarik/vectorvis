#include "transform.h"

Transform Transform::combine(const Transform& parent_transform) const {
    return Transform(
        scale_ * parent_transform.scale_,
        offset_ * parent_transform.scale_ + parent_transform.offset_);
}

void TransformStack::push(const Transform& transform) {
    stack_.push(transform.combine(top()));
}

Transform TransformStack::top() const {
    if (stack_.size() > 0) return stack_.top();

    return Transform(Vec2d(1.0, 1.0), Vec2d(0.0, 0.0));
}
