#ifndef STACK_TESTS_HPP
#define STACK_TESTS_HPP

#include "../lib/data_structures/stack.h"

TEST(Stack, PushPop) {
    Stack<int> stack;
    for (int id = 0; id < 1024; ++id) {
        stack.push(id);
    }
    for (int id = 1023; id >= 0; --id) {
        EXPECT_EQ(stack.top(), id);
        stack.pop();
    }
}

#endif
