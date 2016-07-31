#include "stdafx.h"
#include "Stack.h"

const int integers[] = {10, 1, 2, 5, 4, 3, 7, 72, 8, 19};

class Stack_Fixture 
	: public testing::Test
{
protected:
	typedef light::Stack<int> STACK_INT;

protected:
	virtual void SetUp(void)
	{
		for(int i = 0; i < _countof(integers); i++)
		{
			m_Stack.push(integers[i]);
		}
	}

	virtual void TearDown(void)
	{
	}

protected:
	STACK_INT m_Stack;
};

TEST_F(Stack_Fixture, Iterator)
{
	int count = 0;
	for(STACK_INT::iterator cursor = m_Stack.begin(); cursor != m_Stack.end(); ++cursor)
	{
		int n = *cursor;
		EXPECT_EQ(n, integers[count]);
		++count;
	}
}

TEST_F(Stack_Fixture, PushAndPop)
{
	int count = _countof(integers) - 1;
	while(!m_Stack.empty())
	{
		int n = m_Stack.back();
		m_Stack.pop();

		EXPECT_EQ(n, integers[count]);
		--count;
	}
}

TEST_F(Stack_Fixture, Clear)
{
	m_Stack.clear();
	for(STACK_INT::iterator it = m_Stack.begin(); it != m_Stack.end(); ++it)
	{
		EXPECT_TRUE(0);
	}
}
