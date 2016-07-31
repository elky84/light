#include "stdafx.h"
#include "SystemTimeEx.h"
#include "BuildTime.h"

TEST(BuildTime, Test)
{
#ifdef _DEBUG
#else // _DEBUG
	light::SYSTEMTIME_EX st;
	ASSERT_TRUE( true == light::BuildTime::GetTime(st) );
#endif // _DEBUG
}
