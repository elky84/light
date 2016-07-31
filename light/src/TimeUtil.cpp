#include "light.h"
#include "TimeUtil.h"

namespace light
{

TimeUtil::COMPARE_RESULT::Type TimeUtil::systemtime_compare_YMD(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs)
{
	if (lhs.wYear < rhs.wYear)			
		return COMPARE_RESULT::UNDER;
	else if (lhs.wYear > rhs.wYear)		
		return COMPARE_RESULT::OVER;

	if (lhs.wMonth < rhs.wMonth)		
		return COMPARE_RESULT::UNDER;
	else if( lhs.wMonth > rhs.wMonth)	
		return COMPARE_RESULT::OVER;

	if (lhs.wDay < rhs.wDay)			
		return COMPARE_RESULT::UNDER;
	else if (lhs.wDay > rhs.wDay)		
		return COMPARE_RESULT::OVER;

	return COMPARE_RESULT::EQUAL;
}

TimeUtil::COMPARE_RESULT::Type TimeUtil::systemtime_compare_YMDH(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs)
{
	COMPARE_RESULT::Type Result = systemtime_compare_YMD(lhs, rhs);
	if(COMPARE_RESULT::EQUAL != Result)
		return Result;

	if (lhs.wHour < rhs.wHour)			
		return COMPARE_RESULT::UNDER;
	else if (lhs.wHour > rhs.wHour)		
		return COMPARE_RESULT::OVER;

	return COMPARE_RESULT::EQUAL;
}

TimeUtil::COMPARE_RESULT::Type TimeUtil::systemtime_compare_YMDHM(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs)
{
	COMPARE_RESULT::Type Result = systemtime_compare_YMDH(lhs, rhs);
	if(COMPARE_RESULT::EQUAL != Result)
		return Result;

	if (lhs.wMinute < rhs.wMinute)		
		return COMPARE_RESULT::UNDER;
	else if (lhs.wMinute > rhs.wMinute)	
		return COMPARE_RESULT::OVER;

	return COMPARE_RESULT::EQUAL;
}

TimeUtil::COMPARE_RESULT::Type TimeUtil::systemtime_compare(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs)
{
	COMPARE_RESULT::Type Result = systemtime_compare_YMDH(lhs, rhs);
	if(COMPARE_RESULT::EQUAL != Result)
		return Result;

	if (lhs.wSecond < rhs.wSecond)		
		return COMPARE_RESULT::UNDER;
	else if (lhs.wSecond > rhs.wSecond)	
		return COMPARE_RESULT::OVER;

	if (lhs.wMilliseconds < rhs.wMilliseconds)		
		return COMPARE_RESULT::UNDER;
	else if (lhs.wMilliseconds > rhs.wMilliseconds)	
		return COMPARE_RESULT::OVER;

	return COMPARE_RESULT::EQUAL;
}
	
std::wstring TimeUtil::get_file_timestamp()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	WCHAR szTimeStamp[_MAX_FNAME];
	swprintf_s(szTimeStamp, L"%04d%02d%02d_%02d%02d%02d%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return szTimeStamp;
}


std::wstring TimeUtil::get_timestamp()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return get_timestamp(st);
}

std::wstring TimeUtil::get_timestamp(SYSTEMTIME& st)
{
	WCHAR szTimeStamp[_MAX_FNAME] = {0, };
	swprintf_s(szTimeStamp, L"%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return szTimeStamp;
}

time_t TimeUtil::tm2time_t(IN tm &tmTime)
{
	return  mktime(&tmTime);
}

void TimeUtil::time_t2tm(IN OUT tm &tmTime, IN time_t &time)
{
	 ::localtime_s(&tmTime, &time);
}

time_t TimeUtil::get_local_time_t()
{
	boost::posix_time::ptime ptime = boost::posix_time::second_clock::local_time();
	return ptime_to_time_t(ptime);
}

boost::posix_time::ptime TimeUtil::get_local_time()
{
	return boost::posix_time::second_clock::local_time();
}

int TimeUtil::datetime_to_day_of_week(IN boost::posix_time::ptime& ptime)
{
	tm tmTime = boost::posix_time::to_tm(ptime);
	return tmTime.tm_wday;
}

time_t TimeUtil::ptime_to_time_t(IN boost::posix_time::ptime& ptime)
{
	tm tmTime = boost::posix_time::to_tm(ptime);
	return tm2time_t(tmTime);
}

time_t TimeUtil::datetime_to_time_t(IN std::string str)
{
	boost::posix_time::ptime ptime = boost::posix_time::time_from_string(str);
	return ptime_to_time_t(ptime);
}

TimeUtil::time_d TimeUtil::time_to_time_d(IN std::string str)
{
	return boost::posix_time::duration_from_string(str);
}

#pragma warning(push)
#pragma warning(disable : 4244)

void TimeUtil::tm2TIMESTAMP(IN OUT TIMESTAMP_STRUCT &TimeStamp, IN tm &rtm)
{
	TimeStamp.year		= rtm.tm_year + 1900;
	TimeStamp.month		= rtm.tm_mon + 1;
	TimeStamp.day		= rtm.tm_mday;
	TimeStamp.hour		= rtm.tm_hour;
	TimeStamp.minute	= rtm.tm_min;
	TimeStamp.second	= rtm.tm_sec;
}

#pragma warning(pop)




time_t TimeUtil::TIMESTAMP2time_t(IN TIMESTAMP_STRUCT &TimeStamp)
{
	tm tmTime;
	TIMESTAMP2tm(TimeStamp, tmTime);

	return tm2time_t(tmTime);
}




void TimeUtil::TIMESTAMP2tm(IN OUT TIMESTAMP_STRUCT &TimeStamp, IN tm &tmTime)
{
	tmTime.tm_year = TimeStamp.year - 1900;
	tmTime.tm_mon  = TimeStamp.month - 1;
	tmTime.tm_mday = TimeStamp.day;
	tmTime.tm_hour = TimeStamp.hour;
	tmTime.tm_min  = TimeStamp.minute;
	tmTime.tm_sec  = TimeStamp.second;
}




void TimeUtil::GetLocalTime(IN OUT TIMESTAMP_STRUCT &TimeStamp)
{
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	TimeStamp.year		= sysTime.wYear;
	TimeStamp.month		= sysTime.wMonth;
	TimeStamp.day		= sysTime.wDay;
	TimeStamp.hour		= sysTime.wHour;
	TimeStamp.minute	= sysTime.wMinute;
	TimeStamp.second	= sysTime.wSecond;
}



void TimeUtil::GetLocalTime(IN OUT tm &rtm)
{
	time_t curTime;
	time(&curTime);

	localtime_s(&rtm, &curTime);
}

void TimeUtil::unixtime_to_filetime(DWORD t, FILETIME& ft)
{
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD)ll;
	ft.dwHighDateTime = (DWORD)(ll >> 32);
}

} // namespace light