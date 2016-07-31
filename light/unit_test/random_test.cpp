#include "stdafx.h"
#include "Random.hpp"

#include <vector>

const int SEED = 5000;

TEST(Random, seed)
{
	light::Random::seed(SEED);

	std::vector<short> vector_short1, vector_short2;
	for(int i = 0; i < SEED; ++i)
	{
		vector_short1.push_back(light::Random::rand());
	}

	light::Random::seed(SEED);

	for(int i = 0; i < SEED; ++i)
	{
		vector_short2.push_back(light::Random::rand());
	}

	for(int i = 0; i < SEED; ++i)
	{
		EXPECT_EQ(vector_short1[i], vector_short2[i]);
	}
}

TEST(Random, Float)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		float fValue = light::Random::rand_float();
		EXPECT_GT( fValue, 0.0f );
		EXPECT_LT( fValue, 1.0f);
	}
}

TEST(Random, Max)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Max = SEED / 5;
		int nValue = light::Random::rand(Max);
		EXPECT_GE( nValue, 0 );
		EXPECT_LT( nValue, Max );
	}
}

TEST(Random, MinMax)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Min = 50;
		static const int Max = SEED;
		int nValue = light::Random::rand(Min, Max);
		EXPECT_GE( nValue, Min );
		EXPECT_LT( nValue, Max );
	}
}

TEST(Random, NegativeNumberMinMax)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Min = -50;
		static const int Max = SEED;
		int nValue = light::Random::rand(Min, Max);
		EXPECT_GE( nValue, Min );
		EXPECT_LT( nValue, Max );
	}
}

TEST(Random, ComplexMax)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Max = SEED;
		int nValue = light::Random::complex_rand(Max);
		EXPECT_GE( nValue, 0 );
		EXPECT_LT( nValue, Max );
	}
}

TEST(Random, ComplexMinMax)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Min = SEED / 2;
		static const int Max = SEED;

		int nValue = light::Random::complex_rand(Min, Max);
		EXPECT_GE( nValue, Min );
		EXPECT_LT( nValue, Max );
	}
}

TEST(Random, ComplexNegativeNumberMinMax)
{
	light::Random::seed(SEED);
	for(int i = 0; i < SEED; ++i)
	{
		static const int Min = -SEED;
		static const int Max = SEED;
		int nValue = light::Random::complex_rand(Min, Max);
		EXPECT_GE( nValue, Min );
		EXPECT_LT( nValue, Max );
	}
}
