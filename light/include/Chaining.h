#pragma once

#include "light.h"
#include <boost/function.hpp>
#include "ChainingManager.h"

namespace light
{

typedef boost::function<void()> chaining_callback;

template <typename T, typename KEY>
void chaining_start(KEY key)
{
	ChainingManager<T>::Instance()->start(key);
}

template <typename T, typename KEY>
void chaining_register(KEY key, T* p)
{
	ChainingManager<T>::Instance()->set(key, p);
}

template <typename T>
void chaining_update()
{
	ChainingManager<T>::Instance()->on_update();
}

template <typename T, typename KEY>
size_t chaining_size(KEY key)
{
	return ChainingManager<T>::Instance()->size(key);
}

class Chaining
{
public:
	Chaining(const bool& load, const chaining_callback on_start, const chaining_callback on_timeout, const DWORD timeout = 10000)
		: m_load(load)
		, m_timeout(timeout)
		, m_start_callback(on_start)
		, m_timeout_callback(on_timeout)
		, m_start(0)
	{
	}

	virtual ~Chaining()
	{
	}

	void start()
	{
		if(m_start_callback)
		{
			m_start_callback();
			m_start = GetTickCount();
		}
	}

	const bool is_load() const
	{
		return m_load;
	}

	const bool is_timeout() const
	{
		if(m_timeout == 0) // 0 is infinite.
			return false;

		return (GetTickCount() - m_start) > m_timeout;
	}

	void timeout()
	{
		if(m_timeout_callback)
		{
			m_timeout_callback();
		}
	}

private:
	/// start time value
	DWORD m_start;

	/// timeout condition value
	DWORD m_timeout;

	/// job start callback
	chaining_callback m_start_callback;

	/// timeout callback
	chaining_callback m_timeout_callback;

	/// 참조자로서 들고 있는 이유는, 해당 클래스에 대해서 모르게 하기 위함. 
	const bool& m_load;
};

} // namespace light