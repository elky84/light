#include "stdafx.h"

#include "BoostRandom.h"

TEST(BoostRandom, DoubleTest)
{
	static const double min = 100;
	static const double max = 10000;

	for(int i = 0; i < max - min; ++i)
	{
		double d = light::BoostRandom::rand_double(min, max);
		EXPECT_TRUE(min <= d);
		EXPECT_TRUE(d < max);
	}
}

TEST(BoostRandom, DoubleSameDistanceTest)
{
	static const double min = 100;
	static const double max = 100;

	double d = light::BoostRandom::rand_double(min, max);
	EXPECT_EQ(min, d);
	EXPECT_EQ(d, max);
}


TEST(BoostRandom, DoubleZeroDistanceTest)
{
	static const double min = 0;
	static const double max = 0;

	double d = light::BoostRandom::rand_double(min, max);
	EXPECT_EQ( 0, d );
}


TEST(BoostRandom, IntTest)
{
	static const int min = 100;
	static const int max = 10000;

	for(int i = 0; i < max - min; ++i)
	{
		int n = light::BoostRandom::rand_int(min, max);
		EXPECT_TRUE(min <= n);
		EXPECT_TRUE(n < max);
	}
}

TEST(BoostRandom, IntSameDistanceTest)
{
	static const int min = 100;
	static const int max = 100;

	int n = light::BoostRandom::rand_int(min, max);
	EXPECT_EQ(min, n);
	EXPECT_EQ(n, max);
}

TEST(BoostRandom, IntZeroDistanceTest)
{
	static const int min = 0;
	static const int max = 0;

	int n = light::BoostRandom::rand_int(min, max);
	EXPECT_EQ( 0, n );
}


TEST(BoostRandom, SeedIntTest)
{
	static const int min = 0;
	static const int max = 10000;

	light::BoostRandom::SeedInt seed(100);
	for(int i = 0; i < 10000; ++i)
	{	
		int n = seed.rand(min, max);
		EXPECT_TRUE(min <= n);
		EXPECT_TRUE(n < max);
	}
}

TEST(BoostRandom, SeedIntSameDistanceTest)
{
	static const int min = 100;
	static const int max = 100;

	light::BoostRandom::SeedInt seed(100);
	int n = seed.rand(min, max);
	EXPECT_EQ(min, n);
	EXPECT_EQ(n, max);
}

TEST(BoostRandom, SeedIntZeroDistanceTest)
{
	static const int min = 0;
	static const int max = 0;

	light::BoostRandom::SeedInt seed(100);
	int n = seed.rand(min, max);
	EXPECT_EQ( 0, n );
}

TEST(BoostRandom, SeedIntSameSeedTest)
{
	static const int min = 0;
	static const int max = 10000;

	light::BoostRandom::SeedInt seed1(100);
	light::BoostRandom::SeedInt seed2(100);
	for(int i = 0; i < 10000; ++i)
	{
		EXPECT_EQ(seed1.rand(min, max), seed2.rand(min, max));
	}
}


TEST(BoostRandom, SeedDoubleTest)
{
	static const double min = 0;
	static const double max = 10000;

	light::BoostRandom::SeedDouble seed(100);
	for(int i = 0; i < 10000; ++i)
	{	
		double d = seed.rand(min, max);
		EXPECT_TRUE(min <= d);
		EXPECT_TRUE(d < max);
	}
}

TEST(BoostRandom, SeedDoubleSameDistanceTest)
{
	static const double min = 100;
	static const double max = 100;

	light::BoostRandom::SeedDouble seed(100);
	double d = seed.rand(min, max);
	EXPECT_EQ(min, d);
	EXPECT_EQ(d, max);
}

TEST(BoostRandom, SeedDoubleZeroDistanceTest)
{
	static const double min = 0;
	static const double max = 0;

	light::BoostRandom::SeedDouble seed(100);
	double d = seed.rand(min, max);
	EXPECT_EQ( 0, d );
}

TEST(BoostRandom, SeedDoubleSameSeedTest)
{
	static const double min = 0;
	static const double max = 10000;

	light::BoostRandom::SeedDouble seed1(100);
	light::BoostRandom::SeedDouble seed2(100);
	for(int i = 0; i < 10000; ++i)
	{
		EXPECT_EQ(seed1.rand(min, max), seed2.rand(min, max));
	}
}
