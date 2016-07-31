#pragma once

#include <Windows.h>

namespace light
{

class Semaphore
{
public:
    static const int MAXIMUM_COUNT = 100;

public:
	Semaphore(LONG initial_count, LONG maximum_count = MAXIMUM_COUNT, LPSECURITY_ATTRIBUTES semaphore_attributes = NULL, LPCTSTR name = NULL)
		: m_semaphore(CreateSemaphore(semaphore_attributes, initial_count, maximum_count, name))
	{
		
	}

	~Semaphore()
	{
		CloseHandle(m_semaphore);
	}

	void wait()
	{
		WaitForSingleObject(m_semaphore, INFINITE);
	}

	void release(int release_count = 1)
	{
		ReleaseSemaphore(m_semaphore, release_count, NULL);
	}

private:
    HANDLE m_semaphore;
};

} //namespace light
