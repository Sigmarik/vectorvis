/**
 * @file stack.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Stack data structure
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

template <class T>
struct Stack {
    explicit Stack();
    explicit Stack(const Stack<T>& stack);
    ~Stack();

    Stack<T>& operator=(const Stack<T>& stack);

    void push(const T& value);
    void pop();

    T& top();
    T& operator[](unsigned index);
    size_t size();

   private:
    void relocate(size_t new_capacity);

    T* buffer_;
    size_t size_;
    size_t capacity_;
};

#include "stack.hpp"

#endif
