#pragma once

#include <Windows.h>
#include "TimeUtil.h"

namespace light
{

class SYSTEMTIME_EX 
	: public SYSTEMTIME
{
public:
	SYSTEMTIME_EX()
	{
		reset();
	}

	void reset()
	{
		GetLocalTime(this);
	}

	bool operator ==(const SYSTEMTIME& rhs)
	{
		return Equal(rhs);
	}

	bool operator !=(const SYSTEMTIME& rhs)
	{
		return !Equal(rhs);
	}

	bool operator < (const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::OVER == Compare(rhs);
	}

	bool operator > (const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::UNDER == Compare(rhs);
	}

	bool operator <= (const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::UNDER != Compare(rhs);
	}

	bool operator >= (const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::OVER != Compare(rhs);
	}

	bool EqualYMD(const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::EQUAL == CompareYMD(rhs);
	}

	bool EqualYMDH(const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::EQUAL == CompareYMDH(rhs);
	}

	bool EqualYMDHM(const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::EQUAL == CompareYMDHM(rhs);
	}

	bool Equal(const SYSTEMTIME& rhs)
	{
		return TimeUtil::COMPARE_RESULT::EQUAL == Compare(rhs);
	}

	TimeUtil::COMPARE_RESULT::Type CompareYMD(const SYSTEMTIME& rhs)
	{
		return TimeUtil::systemtime_compare_YMD(*this, rhs);
	}

	TimeUtil::COMPARE_RESULT::Type CompareYMDH(const SYSTEMTIME& rhs)
	{
		return TimeUtil::systemtime_compare_YMDH(*this, rhs);
	}

	TimeUtil::COMPARE_RESULT::Type CompareYMDHM(const SYSTEMTIME& rhs)
	{
		return TimeUtil::systemtime_compare_YMDHM(*this, rhs);
	}

	TimeUtil::COMPARE_RESULT::Type Compare(const SYSTEMTIME& rhs)
	{
		return TimeUtil::systemtime_compare(*this, rhs);
	}

	std::wstring ToString()
	{
		return TimeUtil::get_timestamp(*this);
	}
};

} //namespace light
