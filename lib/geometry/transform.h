/**
 * @file transform.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Transform structure
 * @version 0.1
 * @date 2023-11-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Plug/Math.h"
#include "data_structures/stack.h"

struct Transform {
    Transform(const Vec2d& scale, const Vec2d& offset)
        : scale_(scale), offset_(offset) {}

    Vec2d getOffset() const { return offset_; }
    void setOffset(const Vec2d& offset) { offset_ = offset; }
    Vec2d getScale() const { return scale_; }
    void setScale(const Vec2d& scale) { scale_ = scale; }

    Vec2d apply(const Vec2d& vector) { return vector * scale_ + offset_; }
    Vec2d restore(const Vec2d& vector) { return (vector - offset_) / scale_; }

    Transform combine(const Transform& parent_transform);

   private:
    Vec2d scale_ = Vec2d(1.0, 1.0);
    Vec2d offset_ = Vec2d(0.0, 0.0);
};

struct TransformStack {
    TransformStack();

    void push(const Transform& transform);
    void pop() { stack_.pop(); }
    size_t size() const { return stack_.size(); }
    Transform top() const;

   private:
    Stack<Transform> stack_;
};

#endif
