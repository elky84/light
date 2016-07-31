#include "stdafx.h"
#include "RandomTable.hpp"

#include <vector>

TEST(RandomTable, EqualDivide)
{
	const int DATA_SIZE = 1000;

	light::RandomTable<int, DATA_SIZE> table;
	table.push(0, 200);
	table.push(1, 200);
	table.push(2, 200);
	table.push(3, 200);
	table.push(4, 200);
	table.shuffle();

	std::vector<int> vector;
	vector.resize(5);

	for(int i = 0; i < DATA_SIZE; ++i)
	{
		int data = table.pop();
		ASSERT_TRUE(0 <= data || data < 5);
		++vector[data];
	}

	for(int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(vector[i], 200);
	}
}

TEST(RandomTable, EqualDivideX2)
{
	const int DATA_SIZE = 1000;

	light::RandomTable<int, DATA_SIZE> table;
	table.push(0, 200);
	table.push(1, 200);
	table.push(2, 200);
	table.push(3, 200);
	table.push(4, 200);
	table.shuffle();

	std::vector<int> vector;
	vector.resize(5);

	for(int i = 0; i < DATA_SIZE * 2; ++i)
	{
		int data = table.pop();
        ASSERT_TRUE(0 <= data || data < 5);
		++vector[data];
	}

	for(int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(vector[i], 200 * 2);
	}
}

TEST(RandomTable, GradeDivide)
{
	const int DATA_SIZE = 1000;

	const int rates[] = {3, 7, 30, 70, 890};

	light::RandomTable<int, DATA_SIZE> table;
	table.push(0, 3);
	table.push(1, 7);
	table.push(2, 30);
	table.push(3, 70);
	table.push(4, 890);
	table.shuffle();

	std::vector<int> vector;
	vector.resize(5);

	for(int i = 0; i < DATA_SIZE; ++i)
	{
		int data = table.pop();
		EXPECT_TRUE(0 <= data || data < 5);
		++vector[data];
	}

	for(int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(vector[i], rates[i]);
	}
}
