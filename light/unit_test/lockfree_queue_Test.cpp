#include "stdafx.h"
#include "lockfree_queue.h"

TEST(lockfree_queue, Pop)
{
	const int integers[] = {10, 1, 2, 5, 4, 3, 7, 72, 8, 19};

	light::lockfree_queue<int> queue;
	for(int i = 0; i < _countof(integers); i++)
	{
		queue.push(integers[i]);
	}

	int count = 0;
	while( false == queue.empty())
	{
		int n;
		EXPECT_TRUE( queue.pop(n) );

		EXPECT_EQ(n, integers[count]);
		++count;
	}
}
