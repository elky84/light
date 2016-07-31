#pragma once

#pragma warning ( push )
#pragma warning ( disable : 4819)
#include "boost/random.hpp"
#pragma warning ( pop )

namespace light
{

/**
@brief boost/random wrapping class.
@desc randEngine 및 Distributions 선택은 http://www.boost.org/doc/libs/1_52_0/doc/html/boost_random/performance.html 를 참조하세요. 속도와 분포가 조합에 따라 매우 차이가 납니다.
*/
class BoostRandom
{
private:
	template <typename T, typename Distribution, typename randEngine>
	class Generator
	{
	public:
		Generator(size_t seed, const T min, const T max)
			: m_Generator(randEngine(seed), Distribution(min, max))
		{

		}

		Generator(const T min, const T max)
			: m_Generator(randEngine(), Distribution(min, max))
		{

		}

		T operator()()
		{
			return m_Generator();
		}

	private:
		boost::variate_generator<randEngine, Distribution> m_Generator;
	};

public:
	template <typename T, typename Distribution, typename randEngine = boost::mt19937>
	class seed
	{
	public:
		seed(const size_t seed = 0)
			: m_seed(seed)
		{

		}

		T rand(const T minimum, const T maximum)
		{
			T val = BoostRandom::rand<T, Distribution, randEngine>(m_seed, minimum, maximum);
			m_seed = (size_t)val; // 시드 갱신. 이렇게 해줘야, 다음 값이 바뀜.
			return val;
		}

		void set(const size_t n)
		{
			m_seed = n;
		}

	private:
		size_t m_seed;
	};

	typedef seed<int, boost::uniform_int<> > SeedInt;

	typedef seed<double, boost::uniform_real<> > SeedDouble;

public: // public simple method.
	static int rand_int(const int minimum, const int maximum);

	static double rand_double(const double minimum, const double maximum);

public: // template rand method. if you choice randEngine and Distribution, use to this function.
	template <typename T, typename Distribution, typename randEngine>
	static T rand(const size_t seed, const T minimum, const T maximum)
	{
		if( false == valid(minimum, maximum) ) 
			return minimum;

		Generator< T, Distribution, randEngine > generator(seed, minimum, maximum);
		return generator();
	}

private:
	template <typename T>
	static bool valid(const T minimum, const T maximum)
	{
		if(minimum == maximum) // 같은 범위의 값을 지정하면, 무한 루프에 빠진다. 헐~ 테스트 안해봤으면 뭐 될뻔...
			return false;

		return true;
	}
};

} // namespace light
