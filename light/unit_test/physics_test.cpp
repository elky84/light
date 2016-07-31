#include "stdafx.h"
#include "Physics.hpp"

TEST(Physics, Accelate)
{
	//local floats for holding our data from the user.   
	float velocityInitial = 500.0f, velocityFinal = 200.0f;
	float time = 60.0f; // per second

	EXPECT_EQ(light::Physics::AccelerationSeconds(velocityInitial,velocityFinal, time), (velocityFinal - velocityInitial) / time);
}

TEST(Physics, AverageVelocity)
{
	float StartPos = 100.0f,EndPos = 200.0f;
	float time = 60.0f; // per second

	EXPECT_EQ(light::Physics::AvergageVelocity(StartPos, EndPos, time), (EndPos - StartPos) / time);
}

TEST(Physics, Displacement)
{
	//local floats for holding our data from the user.   
	float velocity     = 126.0f;
	float time         = 60.0f;

	//calculate the displacement and return.
	EXPECT_EQ(light::Physics::Displacement(velocity, time), velocity * time);
}

TEST(Physics, MovePosition)
{
	//local floats for holding our data from the user.   
	float currPosition = 60.0f;
	float velocity     = 15.0f;
	float time         = 60.0f;

	EXPECT_EQ(light::Physics::MovePosition(currPosition, velocity, time), currPosition + (velocity * time));
}
