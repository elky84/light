#pragma once

// http://lists.boost.org/boost-users/2016/04/85968.php
#if _MSC_FULL_VER >= 190023918 // VC++ 2015 Update 2 
#define _ENABLE_ATOMIC_ALIGNMENT_FIX 
#endif 

#include <boost/lockfree/queue.hpp>

namespace light
{

template <typename T>
class lockfree_queue
{
public:
	lockfree_queue(int size = 10)
        : m_queue(size)
	{

	}
	void push(const T& t)
	{
		m_queue.push(t);
	}

	bool pop(T& t)
	{
		if( m_queue.empty())
		{
            return false;
		}

		m_queue.pop(t);
		return true;
	}

	bool empty()
	{
		return m_queue.empty();
	}

private:
	boost::lockfree::queue<T> m_queue;
};

} //namespace light
