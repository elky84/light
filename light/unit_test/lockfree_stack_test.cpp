#include "stdafx.h"
#include "lockfree_stack.h"

TEST(lockfree_stack, Pop)
{
    const int integers[] = { 10, 1, 2, 5, 4, 3, 7, 72, 8, 19 };

    light::lockfree_stack<int> stack;
    for (int i = 0; i < _countof(integers); i++)
    {
        stack.push(integers[i]);
    }

    int count = _countof(integers);
    while (false == stack.empty())
    {
        int n;
        EXPECT_TRUE(stack.pop(n));

        EXPECT_EQ(n, integers[--count]);
    }
}