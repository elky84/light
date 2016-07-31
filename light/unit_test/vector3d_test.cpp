#include "stdafx.h"
#include "Vector.hpp"

light::Vector3D vec1(1.0f, 2.0f, 3.0f);
light::Vector3D vec2(0.5f, 0.8f, 0.2f);

TEST(Vector3D, Addition)
{
	light::Vector3D vec_result =  vec1 + vec2; //∫§≈Õ¿« µ°º¿
	EXPECT_TRUE(vec_result == light::Vector3D(vec1.X() + vec2.X(), vec1.Y() + vec2.Y(), vec1.Z() + vec2.Z()));
}

TEST(Vector3D, Subtract)
{
	light::Vector3D vec_result =  vec1 - vec2; //∫§≈Õ¿« ª¨º¿
	EXPECT_TRUE(vec_result == light::Vector3D(vec1.X() - vec2.X(), vec1.Y() - vec2.Y(), vec1.Z() - vec2.Z()));
}

TEST(Vector3D, Normalize)
{
	float fLength = vec1.get_length(); //∫§≈Õ¿« ±Ê¿Ã: v1¿« ±Ê¿Ã
	EXPECT_EQ(fLength, sqrt(vec1.X()*vec1.X() + vec1.Y() * vec1.Y() + vec1.Z() * vec1.Z()));
	light::Vector3D vNormalize = vec1.Normalize(); //∫§≈Õ¿« ¥‹¿ß∫§≈Õ: v1¿« ¥‹¿ß∫§≈Õ
	EXPECT_TRUE(light::equals(vNormalize.X(), vec1.X() / fLength) && light::equals(vNormalize.Y(), vec1.Y() / fLength) && light::equals(vNormalize.Z(), vec1.Z() / fLength));
}

TEST(Vector3D, Multiply)
{
	light::Vector3D vScale = vec1.Scale(3.0f); //v1ø° Ω∫ƒÆ∂Û 3.0¿ª ∞ˆ«— ∫§≈Õ : k*v1
	EXPECT_TRUE(vScale.X() == vec1.X() * 3.0f && vScale.Y() == vec1.Y() * 3.0f && vScale.Z() == vec1.Z() * 3.0f);
}

TEST(Vector3D, Dot)
{
	float fDot = vec1.Dot(vec2); //∫§≈Õ¿« ≥ª¿˚: v1 . v2
	EXPECT_EQ(fDot, (vec1.X() * vec2.X() + vec1.Y() * vec2.Y() + vec1.Z() * vec2.Z()));
}

TEST(Vector3D, Disjoint)
{
	light::Vector3D vec_result = vec2.Scale( vec1.Dot(vec2) / ( vec2.get_length()*vec2.get_length() )); //∫–«ÿ ∫§≈Õ :  v2ø° ∆Ú«‡«— v1∫§≈Õ¿« ∫–«ÿ∫§≈Õ
}

TEST(Vector3D, Cross)
{
	light::Vector3D vec_result = vec1; //∫§≈Õ 1¿ª ¥Î¿‘.
	vec_result.Cross(vec1, vec2); //∫§≈Õ¿« ø‹¿˚: v1 x v2
}
