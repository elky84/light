#pragma once

#include <Windows.h>

namespace light
{

class Mutex
{
public:
	Mutex(LPSECURITY_ATTRIBUTES mutex_attributes = NULL, BOOL init_owner = FALSE, LPCTSTR name = NULL)
		: m_mutex(CreateMutex(mutex_attributes, init_owner, name))
	{
		
	}

	~Mutex()
	{
		release();
		CloseHandle(m_mutex);
	}

	DWORD acquire()
	{
		return WaitForSingleObject(m_mutex, INFINITE);
	}

	void release()
	{
		ReleaseMutex(m_mutex);
	};

private:
    HANDLE m_mutex;

};

} //namespace light
