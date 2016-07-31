#include "light.h"
#include "PerformanceCounter.h"

namespace light
{

PerfomanceCounter::PerfomanceCounter()
{
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), QueryPerformanceFrequency(&m_frequency));
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
}

PerfomanceCounter::~PerfomanceCounter()
{
}

void PerfomanceCounter::begin()
{
	QueryPerformanceCounter(&m_begin_counter);
}

void PerfomanceCounter::end()
{
	QueryPerformanceCounter(&m_end_counter);
}

double PerfomanceCounter::GetElapsedSecond()
{
	return (double)(m_end_counter.QuadPart - m_begin_counter.QuadPart) / (double)m_frequency.QuadPart;
}

DWORD PerfomanceCounter::GetElapsedMilisecond()
{
	return (DWORD)floor(GetElapsedSecond() * 1000);
}

} //namespace light
