#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include "matrix.h"

template <class Mat>
struct MatrixStack {
    explicit MatrixStack(Mat base);

    void push(Mat matrix);
    void pop();

    Mat& top();
    size_t size();

   private:
    Stack<Mat> stack_;
};

#include "matrix_stack.hpp"

#endif
