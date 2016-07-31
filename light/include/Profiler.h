#pragma once

namespace light
{

class Profiler
{
public:
	static const int DEFAULT_BASELINE = 50;

public:
	Profiler(const WCHAR* name, const DWORD baseline = DEFAULT_BASELINE);

	virtual ~Profiler();

public:
	const bool profile(const WCHAR* log, ...) const;

protected:
	const double elapsed() const;

protected:
	LARGE_INTEGER m_frequency;

	LARGE_INTEGER m_begin_time;

	const WCHAR* m_name;

	const DWORD m_baseline;

    const DWORD m_thread_id;
};

class ScopeProfiler
{
public:
	ScopeProfiler(const WCHAR* name, const DWORD baseline = Profiler::DEFAULT_BASELINE, const WCHAR* log = L"", ...);

	~ScopeProfiler();

private:
	Profiler m_profiler;

	std::wstring m_description;
};

} // namespace light