#pragma once

#include "Queue.h"
#include "spin_lock.h"

namespace light
{

template <typename T>
class DoubleQueue
{
public:
	DoubleQueue()
		: m_consumer(0)
	{

	}
	void push(const T& t)
	{
        scope_lock<spin_lock> lock(m_lock[!m_consumer]);
		m_queue[!m_consumer].push(t);
	}

	bool pop(T& t)
	{
		scope_lock<spin_lock> lock(m_lock[m_consumer]);
		if( true == m_queue[m_consumer].empty())
		{
			swap();
			if( true == m_queue[m_consumer].empty())
			{
				return false;
			}
			else
			{
				return pop(t);
			}
		}

		t = m_queue[m_consumer].front();
		m_queue[m_consumer].pop();
		return true;
	}

	bool empty()
	{
		scope_lock<spin_lock> lock1(m_lock[0]), lock2(m_lock[1]);
		return m_queue[0].empty() && m_queue[1].empty() ? true : false;
	}

private:
	void swap()
	{
        scope_lock<spin_lock> lock(m_lock[!m_consumer]); // consumer lock은 걸고 들어왔다는 전제.
        m_consumer = !m_consumer;
	}

private:
	Queue<T> m_queue[2];

	spin_lock m_lock[2];

	bool m_consumer;
};

} //namespace light
