#pragma once

#include <Windows.h>

#include <time.h>
#include <sql.h>

#include <boost/date_time.hpp>

namespace light
{

class TimeUtil
{
public:
	typedef boost::posix_time::time_duration time_d;

	struct COMPARE_RESULT
	{
		enum Type
		{
			UNDER = -1,
			OVER = 1,
			EQUAL = 0,
		};
	};

public:
	static COMPARE_RESULT::Type systemtime_compare_YMD(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs);

	static COMPARE_RESULT::Type systemtime_compare_YMDH(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs);

	static COMPARE_RESULT::Type systemtime_compare_YMDHM(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs);

	static COMPARE_RESULT::Type systemtime_compare(const SYSTEMTIME & lhs, const SYSTEMTIME & rhs);
	
	static std::wstring get_file_timestamp();

	static std::wstring get_timestamp();

	static std::wstring get_timestamp(SYSTEMTIME& st);

	static void unixtime_to_filetime(DWORD t, FILETIME& ft);

	static time_t	tm2time_t(IN tm &tmTime);
	static void		time_t2tm(IN OUT tm &tmTime, IN time_t &time);
	static void		tm2TIMESTAMP(IN OUT TIMESTAMP_STRUCT &TimeStamp, IN tm &rtm);
	static void		TIMESTAMP2tm(IN OUT TIMESTAMP_STRUCT &TimeStamp, IN tm &tmTime);
	static time_t	TIMESTAMP2time_t(IN TIMESTAMP_STRUCT &TimeStamp);
	static void		GetLocalTime(IN OUT TIMESTAMP_STRUCT &TimeStamp);
	static void		GetLocalTime(IN OUT tm &rtm);

	static time_d	time_to_time_d(IN std::string str);

	static time_t	datetime_to_time_t(IN std::string str);

	static int		datetime_to_day_of_week(IN boost::posix_time::ptime& ptime);

	static time_t get_local_time_t();

	static boost::posix_time::ptime get_local_time();

	static time_t	ptime_to_time_t(IN boost::posix_time::ptime& ptime);
};

} //namespace light
