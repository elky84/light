#pragma once

#include "Vector.hpp"

namespace light
{

template <typename T>
class Position
{
public:
	Position() 
		: x(0)
		, y(0) 
	{
	}

	Position(T X, T Y)
		: x(X)
		, y(Y) 
	{
	}

	Position(const Position<T>& pos) 
		: x(pos.X())
		, y(pos.Y()) 
	{
	}

	bool operator == (const Position<T>& pos) const
	{
		return pos.X() == x && pos.Y() == y;
	}

	bool operator != (const Position<T>& pos) const
	{
		return pos.X() != x || pos.Y() != y;
	}

	bool operator < (const Position<T>& pos) const
	{
		return x + y < pos.X() + pos.Y();
	}

	const Position<T>& operator+=(const Position<T>& pos)
	{
		x += pos.X();
		y += pos.Y();
		return *this;
	}

	const Position<T>& operator-=(const Position<T>& pos)
	{
		x -= pos.X();
		y -= pos.Y();
		return *this;
	}

	Position<T> operator-(const Position<T>& pos) const
	{
		return Position<T>(x - pos.X(), y - pos.Y());
	}

	Position<T> operator+(const Position<T>& pos) const
	{
		return Position<T>(x + pos.X(), y + pos.Y());
	}

	Position<T> operator*(const Position<T>& pos) const
	{
		return Position<T>(x * pos.X(), y * pos.Y());
	}

	Position<T> operator*(const T& pos) const
	{
		return Position<T>(x * pos, y * pos);
	}

	const Position<T>& operator=(const Position<T>& pos) 
	{
		x = pos.X();
		y = pos.Y();
		return *this;
	}
	
	Vector Normal(const Position<T>& rhs)
	{
		Position<T> Target = rhs - *this;
		float Sum = sqrt((Target.X() * Target.X()) + (Target.Y() * Target.Y()));
		return Sum != 0.0f ? Vector(Target.X() / Sum, Target.Y() / Sum) : Vector(0.0f, 0.0f);
	}

	Position<T> Force(const T vForce, const Position<T>& rhs)
	{
		return Force(vForce, Normal(rhs));
	}

	Position<T> Force(const T vForce, const Vector& NormalVector)
	{
		x = x + NormalVector.X() * vForce;
		y = y + NormalVector.Y() * vForce;
		return *this;
	}

	void Clear()
	{
		x = y = 0;
	}

	T X() const
	{
		return x;
	}

	T Y() const
	{
		return y;
	}

	void X(T t)
	{
		x = t;
	}

	void Y(T t)
	{
		y = t;
	}

private:
	T x, y;
};

template <typename T>
class Position3D
{
public:
	/// no puts any thing.
	Position3D() 
		: x(0)
		, y(0) 
		, z(0)
	{
	}

	Position3D(T X, T Y, T Z)
		: x(X)
		, y(Y) 
		, z(Z)
	{
	}

	Position3D(const Position3D<T>& pos) 
		: x(pos.X())
		, y(pos.Y()) 
		, z(pos.Z())
	{
	}

	bool operator == (const Position3D<T>& pos) const
	{
		return pos.X() == x && pos.Y() == y && pos.Z() == z;
	}

	bool operator != (const Position3D<T>& pos) const
	{
		return pos.X() != x || pos.Y() != y;
	}

	const Position3D<T>& operator+=(const Position3D<T>& pos)
	{
		x += pos.X();
		y += pos.Y();
		return *this;
	}

	const Position3D<T>& operator-=(const Position3D<T>& pos)
	{
		x -= pos.X();
		y -= pos.Y();
		return *this;
	}

	Position3D<T> operator-(const Position3D<T>& pos) const
	{
		return Position3D<T>(x - pos.X(), y - pos.Y(), z - pos.Z());
	}

	Position3D<T> operator+(const Position3D<T>& pos) const
	{
		return Position3D<T>(x + pos.X(), y + pos.Y(), z + pos.Z());
	}

	Position3D<T> operator*(const Position3D<T>& pos) const
	{
		return Position3D<T>(x * pos.X(), y * pos.Y(), z * pos.Z());
	}

	Position3D<T> operator*(const T& pos) const
	{
		return Position3D<T>(x * pos, y * pos, z * pos);
	}

	const Position3D<T>& operator=(const Position3D<T>& pos) 
	{
		x = pos.X();
		y = pos.Y();
		z = pos.Z();
		return *this;
	}

	Position3D<T> Force(const T vForce, const Vector3D& NormalVector)
	{
		x = x + NormalVector.X() * vForce;
		y = y + NormalVector.Y() * vForce;
		z = z + NormalVector.Z() * vForce;
		return *this;
	}

	T X() const
	{
		return x;
	}

	T Y() const
	{
		return y;
	}

	T Z() const
	{
		return z;
	}

	void X(T t)
	{
		x = t;
	}

	void Y(T t)
	{
		y = t;
	}

	void Z(T t)
	{
		z = t;
	}

private:
	T x, y, z;
};

typedef Position<WORD> POSITION_WORD;
typedef Position<int> POSITION_INT;
typedef Position<float> POSITION_FLOAT;

} //namespace light
