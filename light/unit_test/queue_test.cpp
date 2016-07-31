#include "stdafx.h"
#include "Queue.h"

const int integers[] = {10, 1, 2, 5, 4, 3, 7, 72, 8, 19};

class Queue_Fixture 
	: public testing::Test
{
protected:
	typedef light::Queue<int> QUEUE_INT;

protected:
	virtual void SetUp(void)
	{
		for(int i = 0; i < _countof(integers); i++)
		{
			m_Queue.push(integers[i]);
		}
	}

	virtual void TearDown(void)
	{
	}

protected:
	light::Queue<int> m_Queue;
};

TEST_F(Queue_Fixture, Iterator)
{

	int count = 0;
	for(QUEUE_INT::iterator cursor = m_Queue.begin(); cursor != m_Queue.end(); ++cursor)
	{
		int n = *cursor;
		EXPECT_EQ(n, integers[count]);
		++count;
	}
}

TEST_F(Queue_Fixture, FrontAndPop)
{
	int count = 0;
	while( false == m_Queue.empty())
	{
		int n = m_Queue.front();
		m_Queue.pop();

		EXPECT_EQ(n, integers[count]);
		++count;
	}
}

TEST_F(Queue_Fixture, ClearTest)
{
	m_Queue.clear();
	for(QUEUE_INT::iterator it = m_Queue.begin(); it != m_Queue.end(); ++it)
	{
		EXPECT_TRUE(0);
	}
}
