#pragma once

#define _USE_MATH_DEFINES 

#include <math.h>
#include <float.h>

#include <math.h>

#include <memory.h>

namespace light
{

template <typename T>
static T Rad2Deg( T radian )
{
	return (T)(radian * (180.0f / M_PI));
}

template <typename T>
static T Deg2Rad( T degrees )
{
	return (T)(degrees * (M_PI / 180.0f));
}

template <typename T>
static T LookAtToDegree( const T x, const T y )
{
	float fRad = atan2(x, y);
	float fDegree = Rad2Deg(fRad);
	fDegree = -fDegree;
	if(fDegree < 0)
		fDegree = 360 + fDegree;

	return fDegree;
}

template <typename T>
struct STRUCT_MEMSET_THIS : public T
{
	STRUCT_MEMSET_THIS()
	{
		memset(this, 0, sizeof(*this));
	}
};

template <typename R, typename T1, typename T2>
inline R delta(T1 prev, T2 cur)
{
	return cur < static_cast<T2>(prev) ? ((std::numeric_limits<T2>::max)() - static_cast<T2>(prev)) + cur + 1 : cur - static_cast<T2>(prev);
}

static bool equals(const float a, const float b) 
{
	const float FLOAT_EPSILON  = 0.001f;
	return fabs(a - b) < FLOAT_EPSILON ? true : false;
}

static bool equals(const double a, const double b) 
{
	const double DOUBLE_EPSILON  = 0.001;
	return fabs(a - b) < DOUBLE_EPSILON ? true : false;
}

template <typename T> 
static void safe_release(T t)
{
	if( t )
	{
		t->Release();
		t = 0;
	}
}

template <typename T> 
static void safe_delete(T t)
{
	if( t )
	{
		delete t;
		t = 0;
	}
}

template <typename T>
static T& cast(void* ptr)
{
	return *reinterpret_cast<T*>(ptr);
}

template <typename T>
static T* cast_ptr(void* ptr)
{
	return reinterpret_cast<T*>(ptr);
}

template <typename T>
void Swap(T& objA, T& objB)
{
    T objC = objA;
    objA = objB;
    objB = objC;
}

} //namespace light
