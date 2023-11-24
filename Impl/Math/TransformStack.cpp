#include "TransformStack.h"

void TransformStack::enter(const plug::Transform& transform) {
    stack_.push(transform.combine(top()));
}

plug::Transform TransformStack::top() const {
    if (stack_.size() > 0) return stack_.top();

    return plug::Transform(Vec2d(0.0, 0.0), Vec2d(1.0, 1.0));
}
