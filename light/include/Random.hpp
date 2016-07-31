#pragma once

#include <time.h>

namespace light
{

class Random
{
private:
	Random() 
	{
	}

private:
	static unsigned int m_uSeed;

public:
	static void seed(unsigned int unSeed)
	{
		m_uSeed = unSeed;
	}

	static float rand_float() 
	{
		return static_cast<float>(rand()) / 0x7fff;
	}

	static short rand()
	{
		return static_cast<short>(((m_uSeed = m_uSeed * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	template<typename T>
	static T rand(T Max)
	{
		return rand() % Max;
	}

	template<typename T>
	static T rand(T Min, T Max)
	{
		int Value = (int)(Max - Min);
		return Value != 0 ? rand() % Value + Min : Min;
	}

	static int complex_rand()
	{
		int result;
		m_uSeed *= 1103515245;
		m_uSeed += 12345;
		result = (m_uSeed / 65536) % 2048;

		m_uSeed *= 1103515245;
		m_uSeed += 12345;
		result <<= 10;
		result ^= (m_uSeed / 65536) % 1024;

		m_uSeed *= 1103515245;
		m_uSeed += 12345;
		result <<= 10;
		result ^= (m_uSeed / 65536) % 1024;

		m_uSeed = m_uSeed;
		return result;
	}

	template<typename T>
	static int complex_rand(T Max)
	{
		return complex_rand() % Max;
	}

	template<typename T>
	static int complex_rand(T Min, T Max)
	{
		return complex_rand() % (Max - Min) + Min;
	}
};

__declspec(selectany) unsigned int Random::m_uSeed = static_cast<unsigned int>(::time(NULL));

} //namespace light
