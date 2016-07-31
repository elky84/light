#pragma once

#include <Windows.h>

namespace light
{


class Event  
{
public:
	Event(BOOL manual_reset = FALSE, BOOL initial_state = FALSE, LPSECURITY_ATTRIBUTES event_attributes = NULL,  LPCTSTR name = NULL)
		: m_event(CreateEvent(event_attributes, manual_reset, initial_state, name))
	{
		
	}

	~Event()
	{
		CloseHandle(m_event);
	}

	void reset()
	{
		ResetEvent(m_event);
	}

	BOOL set()
	{
		return SetEvent(m_event);
	}

    DWORD wait(DWORD sec=INFINITE)
	{
		return WaitForSingleObject(m_event, sec);
	}

private:
    HANDLE m_event;
};

} //namespace light
