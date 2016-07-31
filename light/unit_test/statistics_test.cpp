#include "stdafx.h"
#include "Statistics.h"

#include <time.h>

class StatisticsMinMax_Fixture : public testing::Test
{
protected:
	static const int MAX_VALUE = 10000;
	static const int TEST_ELEMENTS = 1000;

public:
	virtual void SetUp(void)
	{
		srand((unsigned int)time(NULL));
	}

	virtual void TearDown(void)
	{
	}
};

TEST_F(StatisticsMinMax_Fixture , Int)
{
	light::StatisticsMinMax<int> statistics;
	for(int i = 0; i < TEST_ELEMENTS; ++i)
	{
        statistics.record(rand() % MAX_VALUE);
	}

	EXPECT_TRUE(statistics.maximum() < MAX_VALUE);
	EXPECT_TRUE( 0 <= statistics.minimum());
	EXPECT_TRUE( TEST_ELEMENTS == statistics.count() );
}

TEST_F(StatisticsMinMax_Fixture , Float)
{
	srand((unsigned int)time(NULL));
	light::StatisticsMinMax<float> statistics;
	for(int i = 0; i < TEST_ELEMENTS; ++i)
	{
		statistics.record((float)(rand() % MAX_VALUE));
	}

	EXPECT_TRUE( statistics.maximum() < MAX_VALUE);
	EXPECT_TRUE( 0.0f <= statistics.minimum());
	EXPECT_TRUE( TEST_ELEMENTS == statistics.count() );
}

TEST_F(StatisticsMinMax_Fixture , Average)
{
	srand((unsigned int)time(NULL));
	light::StatisticsMinMax<float> statistics;
	for(int i = 0; i < TEST_ELEMENTS; ++i)
	{
		float src = (float)(rand() % MAX_VALUE); 
		float result = (float)(rand() % MAX_VALUE);
		statistics.record(result == 0.0f ? src : src / result);
	}

	EXPECT_TRUE( statistics.maximum() < MAX_VALUE);
	EXPECT_TRUE( 0.0f <= statistics.minimum());
	EXPECT_TRUE( TEST_ELEMENTS == statistics.count() );
}



class StatisticsSuccess_Fixture : public testing::Test
{
protected:
	static const int TEST_ELEMENTS = 1000;

public:
	virtual void SetUp(void)
	{
	}

	virtual void TearDown(void)
	{
	}
};

TEST_F(StatisticsSuccess_Fixture, Test)
{
	light::StatisticsSuccess<bool> statistics;
	for(int i = 0; i < TEST_ELEMENTS; ++i)
	{
		statistics.record(true);
	}

	statistics.record(false);

	EXPECT_EQ( statistics.success(), TEST_ELEMENTS );
	EXPECT_EQ( statistics.fail(), 1 );
	EXPECT_EQ( TEST_ELEMENTS + 1, statistics.count() );
}
