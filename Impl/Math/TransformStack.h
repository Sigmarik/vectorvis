/**
 * @file TransformStack.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Transform stack implementation
 * @version 0.1
 * @date 2023-11-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IMPL_MATH_TRANSFORM_STACK_H
#define __IMPL_MATH_TRANSFORM_STACK_H

#include "Plug/Math/Transform.h"
#include "Plug/Math/TransformStack.h"
#include "data_structures/stack.h"

struct TransformStack : public plug::TransformStack {
    void enter(const plug::Transform &transform) override;

    void leave() override {
        if (stack_.size() > 0) stack_.pop();
    }

    plug::Transform top() const override;

    Vec2d apply(const Vec2d &vec) const override { return top().apply(vec); }

    Vec2d restore(const Vec2d &vec) const override {
        return top().restore(vec);
    }

   private:
    Stack<plug::Transform> stack_ = Stack<plug::Transform>();
};

#endif
