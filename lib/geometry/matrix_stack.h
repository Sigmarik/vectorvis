#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include "matrix.h"

template <class Mat>
struct MatrixStack {
    explicit MatrixStack(const Mat& base);

    void push(const Mat& matrix);
    void pop();

    Mat& top() const;
    size_t size() const;

   private:
    Stack<Mat> stack_;
};

#include "matrix_stack.hpp"

#endif
