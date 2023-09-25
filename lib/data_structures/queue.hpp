/**
 * @file queue.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Simple stack-based queue data structure
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "stack.h"

template <class T>
struct Queue {
    Queue() : top_(), bottom_() {}

    void push(const T& value) { top_.push(value); }
    void pop() {
        if (bottom_.size() == 0) {
            while (top_.size() > 0) {
                bottom_.push(top_[top_.size() - 1]);
                top_.pop();
            }
        }

        bottom_.pop();
    }

    size_t size() const { return top_.size() + bottom_.size(); }

    T& operator[](size_t index) const {
        if (index < bottom_.size()) {
            return bottom_[bottom_.size() - index - 1];
        }
        return top_[index - bottom_.size()];
    }

   private:
    Stack<T> top_;
    Stack<T> bottom_;
};

#endif
