#pragma once

namespace light
{

class Physics
{
public:
	//This function will calculate the acceleration in seconds.
	static float AccelerationSeconds( float startVel, float finalVel, float time)
	{
		return (finalVel-startVel)/time;
	}

	//Passing through floats for velocity and time we return the resulting displacement.
	static float AvergageVelocity(float start,float end, float time)
	{
		return (end - start)/ time;
	}

	//Passing through floats for velocity and time we return the resulting displacement.
	static float Displacement(float vel, float time)
	{
		return vel*time;
	}

	//Passing through floats for velocity and time we return the resulting position
	//at the time specified.
	static float MovePosition(float oldPosition, float vel, float time)
	{
		return oldPosition + (vel*time);
	}
};

} //namespace light