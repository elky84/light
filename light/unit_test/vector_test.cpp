#include "stdafx.h"
#include "Vector.hpp"

light::Vector vec1(1.0f, 2.0f);
light::Vector vec2(0.5f, 0.8f);

TEST(Vector, Addition)
{
	light::Vector vec_result =  vec1 + vec2; //∫§≈Õ¿« µ°º¿
	ASSERT_TRUE(vec_result == light::Vector(vec1.X() + vec2.X(), vec1.Y() + vec2.Y()));
}

TEST(Vector, Subtract)
{
	light::Vector vec_result =  vec1 - vec2; //∫§≈Õ¿« ª¨º¿
	ASSERT_TRUE(vec_result == light::Vector(vec1.X() - vec2.X(), vec1.Y() - vec2.Y()));
}

TEST(Vector, Normalize)
{
	float fLength = vec1.get_length(); //∫§≈Õ¿« ±Ê¿Ã: v1¿« ±Ê¿Ã
	ASSERT_EQ(fLength, sqrt(vec1.X()*vec1.X() + vec1.Y() * vec1.Y()));
	light::Vector vNormalize = vec1.Normalize(); //∫§≈Õ¿« ¥‹¿ß∫§≈Õ: v1¿« ¥‹¿ß∫§≈Õ
	ASSERT_TRUE(light::equals(vNormalize.X(), vec1.X() / fLength) && light::equals(vNormalize.Y(), vec1.Y() / fLength));
}

TEST(Vector, Multiply)
{
	light::Vector vScale1 = vec1.Scale(3.0f); //v1ø° Ω∫ƒÆ∂Û 3.0¿ª ∞ˆ«— ∫§≈Õ : k*v1
	ASSERT_TRUE(vScale1.X() == vec1.X() * 3.0f && vScale1.Y() == vec1.Y() * 3.0f);
}

TEST(Vector, Dot)
{
	float fDot = vec1.Dot(vec2); //∫§≈Õ¿« ≥ª¿˚: v1 . v2
	ASSERT_EQ(fDot, (vec1.X() * vec2.X() + vec1.Y() * vec2.Y()));
}

TEST(Vector, Disjoint)
{
	light::Vector vec_result = vec2.Scale( vec1.Dot(vec2) / ( vec2.get_length()*vec2.get_length() )); //∫–«ÿ ∫§≈Õ :  v2ø° ∆Ú«‡«— v1∫§≈Õ¿« ∫–«ÿ∫§≈Õ
}
