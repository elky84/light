#include "stdafx.h"
#include "Position.h"

TEST(Position, ForceVector)
{
	light::Position<float> pos1(5.0f, 5.0f);
	const light::Position<float> pos2(15.0f, 10.0f);

	light::Position<float> posGap = pos2 - pos1;
	EXPECT_TRUE(posGap == light::Position<float>(pos2.X() - pos1.X(), pos2.Y() - pos1.Y()));

	light::Vector vPos1ToPos2(posGap.X(), posGap.Y());

	light::Vector vNormal = vPos1ToPos2.Normalize();

	const float fForce = 1.0f;

	for(int i = 0; i < 25; ++i)
	{
		pos1.Force(fForce, vNormal);
	}
}

TEST(Position, Force)
{
	light::Position<float> pos1(5.0f, 5.0f);
	const light::Position<float> pos2(15.0f, 10.0f);

	const float fForce = 1.0f;
	for(int i = 0; i < 25; ++i)
	{
		pos1.Force(fForce, pos2);
	}
}
	
TEST(Position, 3D)
{
	light::Position3D<float> pos1(5.0f, 5.0f, 5.0f);
	const light::Position3D<float> pos2(15.0f, 10.0f, 5.0f);

	light::Position3D<float> posGap = pos2 - pos1;
	EXPECT_TRUE(posGap == light::Position3D<float>(pos2.X() - pos1.X(), pos2.Y() - pos1.Y(), pos2.Z() - pos1.Z()));

	light::Vector3D vPos1ToPos2(posGap.X(), posGap.Y(), posGap.Z());
	const float fForce = 1.0f;

	light::Vector3D vNormal = vPos1ToPos2.Normalize();

	for(int i = 0; i < 25; ++i)
	{
		pos1.Force(fForce, vNormal);
	}
}
