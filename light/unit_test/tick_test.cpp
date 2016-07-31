#include "stdafx.h"
#include "Tick.h"

TEST(Tick, Test)
{
	const DWORD dwCheckTick = 100;

	DWORD dwBeginTick = GetTickCount();
	light::Tick tick(dwCheckTick);
	while(true == tick.Check(GetTickCount()))
	{
	}

	DWORD dwElapsedTick = GetTickCount() - dwBeginTick;

	EXPECT_TRUE(dwCheckTick <= dwElapsedTick);
}
