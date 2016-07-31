#include "stdafx.h"
#include "List.h"

const int integers[] = {10, 1, 2, 5, 4, 3, 7, 72, 8, 19};

class List_Fixture 
	: public testing::Test
{
protected:
	typedef light::List<int> LIST_INT;

protected:
	virtual void SetUp(void)
	{
		for(int i = 0; i < _countof(integers); i++)
		{
			m_List.PushBack(integers[i]);
		}
	}

	virtual void TearDown(void)
	{
	}

protected:
	LIST_INT m_List;
};

TEST_F(List_Fixture, Iterator)
{
	int count = 0;
	for(LIST_INT::iterator cursor = m_List.begin(); cursor != m_List.end(); ++cursor)
	{
		int n = *cursor;
		EXPECT_EQ(n, integers[count]);
		++count;
	}
}

TEST_F(List_Fixture, PopBack)
{
	int count = _countof(integers) - 1;
	while(!m_List.empty())
	{
		int n = m_List.back();
		m_List.pop_back();

		EXPECT_EQ(n, integers[count]);
		--count;
	}
}

TEST_F(List_Fixture, PopFront)
{
	int count = 0;
	while(!m_List.empty())
	{
		int n = m_List.front();
		m_List.pop_front();

		EXPECT_EQ(n, integers[count]);
		++count;
	}
}

TEST_F(List_Fixture, Clear)
{
	m_List.clear();
	for(LIST_INT::iterator it = m_List.begin(); it != m_List.end(); ++it)
	{
		ASSERT_TRUE(0);
	}
}
