#include "stdafx.h"
#include "DoubleQueue.h"

TEST(DoubleQueue, Pop)
{
	const int aryInteger[] = {10, 1, 2, 5, 4, 3, 7, 72, 8, 19};

	light::DoubleQueue<int> queue;
	for(int i = 0; i < _countof(aryInteger); i++)
	{
		queue.push(aryInteger[i]);
	}

	int count = 0;
	while( false == queue.empty())
	{
		int n;
		EXPECT_TRUE( queue.pop(n) );

		EXPECT_EQ(n, aryInteger[count]);
		++count;
	}
}
