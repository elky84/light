#pragma once

#include <math.h>

namespace light
{

class Vector
{
public:
	Vector();

	Vector(float fX, float fY);

	Vector(const Vector& rhs);

	~Vector();

	float Dot(const Vector& rhs);

	Vector Scale(float k);

	float get_length();

	Vector Normalize();

	Vector operator+(const Vector& rhs);

	Vector operator-(const Vector& rhs);

	void operator=(const Vector& rhs);		

	bool operator==(const Vector& rhs) const;

public:
	float X() const 
	{
		return x;
	}

	float Y() const 
	{
		return y;
	}

private:
	float x;

	float y;
};


class Vector3D
{
public:
	Vector3D();

	Vector3D(float fX, float fY, float fZ);

	Vector3D(const Vector3D& rhs);

	~Vector3D();

	float Dot(const Vector3D& rhs);

	void Cross(const Vector3D& u, const Vector3D& v);

	Vector3D Scale(float k);

	float get_length();

	Vector3D Normalize();

public:
	Vector3D operator+(const Vector3D& rhs);

	Vector3D operator-(const Vector3D& rhs);

	void operator=(const Vector3D& rhs);		

	bool operator==(const Vector3D& rhs) const;

public:
	float X() const 
	{
		return x;
	}

	float Y() const 
	{
		return y;
	}

	float Z() const 
	{
		return z;
	}

private:
	float x;

	float y;

	float z;
};


} //namespace light
