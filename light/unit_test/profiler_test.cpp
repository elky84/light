#include "stdafx.h"

#include "Profiler.h"

#include <boost/bind.hpp>

#define VA_ARGS(STR, ...) STR,##__VA_ARGS__

class MockProfiler : public light::Profiler
{
public:
	MockProfiler(const TCHAR* name, const DWORD baseline)
		: light::Profiler(name, baseline)
	{
	}

	virtual ~MockProfiler()
	{
	}

public:
	const bool profile(const TCHAR* message, ...) const
	{
		return __super::profile( VA_ARGS(message) );
	}

	const double elapsed() const
	{
		return __super::elapsed();
	}

	const DWORD baseline() const
	{
		return m_baseline;
	}
};

TEST(Profiler, ProfileMethodTest)
{
 	MockProfiler profiler(_T("UnitTest"), 100); // 기준치가 100이고, 0~19 * 10 만큼 Sleep하니 대략 50%에 가깝게 성공해야 함
 	for(int n = 0; n < 10 ; ++n)
 	{
 		int nRand = rand() % 20;
 		Sleep(nRand);
 	}
 
 	if(profiler.baseline() <= profiler.elapsed() )
 	{
 		ASSERT_TRUE(profiler.profile(__FUNCTIONW__));
 	}
 	else
 	{
 		ASSERT_FALSE(profiler.profile(__FUNCTIONW__));
 	}
}

 
TEST(Profiler, HaveToSuccessTest) // 반드시 성공 해야 되는 테스트
{
 	MockProfiler profiler(_T("HaveToSuccessTest"), 200); // 기준치 200. 300만큼 Sleep하니 반드시 true가 반환되어야 함
 	Sleep(300);
 
	ASSERT_TRUE(profiler.profile(__FUNCTIONW__));
}