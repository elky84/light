#pragma once

#include <Windows.h>
#include "scope_lock.h"

namespace light
{

class critical_section  
{
public:

	critical_section()
	{
		::InitializeCriticalSection(&m_cs);
	}

	~critical_section()
	{
		::DeleteCriticalSection(&m_cs);
	}

	void lock()	
	{
		::EnterCriticalSection(&m_cs);
	}

	void unlock()
	{
		::LeaveCriticalSection(&m_cs);
	}

	BOOL try_enter()
	{
		return TryEnterCriticalSection(&m_cs);
	}

private:
    CRITICAL_SECTION m_cs;
};


} //namespace light
