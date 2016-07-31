#include "light.h"
#include "Profiler.h"
#include <boost/limits.hpp>

namespace light
{

Profiler::Profiler(const WCHAR* name, const DWORD baseline)
	: m_name(name)
	, m_baseline(baseline)
    , m_thread_id(GetCurrentThreadId())
{
	QueryPerformanceFrequency( &m_frequency );
	QueryPerformanceCounter( &m_begin_time );
}

Profiler::~Profiler()
{
}

const bool Profiler::profile(const WCHAR* log, ...) const
{
	const double duringTime = elapsed();
	if ( duringTime < m_baseline ) // baseline 보다 낮으면 오케바리~
	{
		return false;
	}

	va_list args;
	WCHAR szTextFormatted[1024] = {0, };

	va_start(args, log);
	_vstprintf_s(szTextFormatted, log, args);
	va_end(args);

	LOG_INFO(L"Thread[%u] get_name[%s] ElapsedTick[%f] Desc[%s]", m_thread_id, m_name, duringTime, szTextFormatted);
	return true;
}

const double Profiler::elapsed() const
{
	LARGE_INTEGER Endtime;
	QueryPerformanceCounter( &Endtime );
	__int64 elapsed = Endtime.QuadPart- m_begin_time.QuadPart;
	return (double)elapsed / (double)m_frequency.QuadPart * 1000;
}

ScopeProfiler::ScopeProfiler(const WCHAR* name, const DWORD baseline, const WCHAR* log, ...)
	: m_profiler(name, baseline)
{
	va_list args;
	WCHAR szTextFormatted[1024] = {0, };

	va_start(args, log);
	_vstprintf_s(szTextFormatted, log, args);
	va_end(args);

	m_description = szTextFormatted;
}

ScopeProfiler::~ScopeProfiler()
{
	m_profiler.profile(L"%s", m_description.c_str());
}

} // namespace light