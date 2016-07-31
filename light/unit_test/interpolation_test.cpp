#include "stdafx.h"
#include "Interpolation.h"

TEST(Interpolation, LinearInterpolation)
{
	light::LinearInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, SmoothStepInterpolation)
{
	light::SmoothStepInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, SquaredInterpolation)
{
	light::SquaredInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, InverseSquaredInterpolation)
{
	light::InverseSquaredInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, SinInterpolation)
{
	light::SinInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, CosInterpolation)
{
	light::CosInterpolation interpolation(0.0f, 1.0f, 100);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, WeightedAverageInterpolation)
{
	light::WeightedAverageInterpolation interpolation(0.0f, 1.0f, 100, 2);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}

TEST(Interpolation, SplineInterpolation)
{
	light::SplineInterpolation interpolation(0.0f, 1.0f, 100, 10, 0);
	while_limit( 100, false == interpolation.is_complete() )
	{
		interpolation.execute();
	}
}
