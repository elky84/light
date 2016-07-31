#pragma once

#include "Thread.h"
#include "spin_lock.h"

#include <map>

namespace light
{

namespace net
{

template <typename T>
class EventWorker 
{
	std::map<HANDLE, T*> m_objects;

	spin_lock m_lock;

	std::vector<HANDLE> m_handles;

    Thread m_thread;

	static const DWORD WAIT_TIME = 100;

public:
	EventWorker()
        : m_thread(boost::bind(&EventWorker::on_update, this))
	{
	}

	virtual ~EventWorker()
	{
        m_thread.stop();
	}

    void start()
    {
        m_thread.start();
    }

    void stop()
    {
        m_thread.stop();
    }

	void on_update()
	{
		std::vector<HANDLE> vector_Handle;
		{
			scope_lock<spin_lock> cs(m_lock);
			vector_Handle = m_handles;
			if( vector_Handle.empty() ) // 오브젝트가 0개면 WaitForMultipleObjects 로 대기하지 않음.
			{
				Sleep(WAIT_TIME);
				return;
			}
		}

		DWORD dwIndex = WaitForMultipleObjects(vector_Handle.size(), &vector_Handle[0], FALSE, WAIT_TIME);
		dwIndex = dwIndex - WAIT_OBJECT_0;
		if( WAIT_FAILED == dwIndex )
		{
			LOG_ERROR(L"WAIT_FAILED [%u]", GetLastError());
			for each(auto& val in m_objects)
			{
				val.second->Proc(); // 이렇게 처리를 요청하면, 상황에 따른 처리 (주로 연결 해제 등의)를 할 것이다.
			}
			return;
		}
		else if( WAIT_TIMEOUT == dwIndex )
		{
			return;
		}
		else if(false == IS_VALID_VALUE(dwIndex, 0, vector_Handle.size())) //배열 유효값 검사
		{
			LOG_ERROR(L"Invalid index [%u]", dwIndex);
			return;
		}

        scope_lock<spin_lock> cs(m_lock);
		HANDLE handle = vector_Handle[dwIndex];
		if(m_objects[handle] == NULL)
			return;

		m_objects[handle]->Proc();
	}

	bool set(T* t)
	{
        scope_lock<spin_lock> cs(m_lock);
		m_handles.push_back(t->get_event());
		m_objects[t->get_event()] = t;
		return true;
	}

	bool remove(T* t)
	{
        scope_lock<spin_lock> cs(m_lock);
		auto it = find(m_handles.begin(), m_handles.end(), t->get_event());
		if(it == m_handles.end())
			return false;

		m_handles.erase(it);
		m_objects.erase(t->get_event());
		return true;
	}
};

} // namespace net

} // namespace light
