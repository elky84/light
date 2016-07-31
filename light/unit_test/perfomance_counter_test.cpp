#include "stdafx.h"
#include "PerformanceCounter.h"

void Blah()
{
	// 1ms 만큼 재운다.
	Sleep(1);
}

const int LOOP_COUNT = 10;
const double END_TIME = 100;

TEST(PerfomanceCounter, Test1)
{
	light::PerfomanceCounter counter;
	counter.begin();
	for(int i = 0; i < LOOP_COUNT; ++i)
	{
		Blah();
		Sleep((DWORD)(END_TIME / LOOP_COUNT));
	}
	counter.end();

	EXPECT_LE(END_TIME, counter.GetElapsedMilisecond());
}

TEST(PerfomanceCounter, Test2)
{
	light::PerfomanceCounter counter, loopcounter;
	counter.begin();
	for(int i = 0; i < LOOP_COUNT; ++i)
	{
		loopcounter.begin();
		Blah();
		loopcounter.end();	

		Sleep((DWORD)(ceil(END_TIME / LOOP_COUNT) - floor(loopcounter.GetElapsedSecond()))); // Blah에서 소비된 시간만큼 덜 재운다.
	}

	counter.end();
	EXPECT_LE(END_TIME, counter.GetElapsedMilisecond());
}
