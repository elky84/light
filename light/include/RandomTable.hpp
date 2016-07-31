#pragma once

#include <array>
#include "Seh.h"

namespace light
{

template <typename T, size_t type_size>
class RandomTable
{
public:
	RandomTable()
		: m_push_cursor(0)
		, m_pop_cursor(0)
	{
	}

	~RandomTable()
	{
	}

	bool push(const T& data, size_t count)
	{
		if(m_array.size() < (size_t)((m_array.size() - m_push_cursor) - count)) //허용 가능 범위보다 크게 설정하려함
		{
			LOG_ERROR(L"get_size is Over. [table_size:%u] [size:%u] [count:%u]", m_array.size(), m_array.size(), count);
			return false;
		}

		for(auto n = m_push_cursor ; n < m_push_cursor + count; ++n)
		{
            m_array[n] = data;
		}

		m_push_cursor += count;
		return true;
	}

	T pop()
	{
		if( m_array.size() <= m_pop_cursor)
		{
            shuffle();
			m_pop_cursor = 0;
		}

		return m_array[m_pop_cursor++];
	}

	void shuffle()
	{
		for(auto it = m_array.begin(); it != m_array.end(); ++it)
		{
			swap(*it, *(m_array.begin() + rand() % m_array.size()));
		}
	}

	const size_t get_size()
	{
		return m_array.size();
	}

private:
	void swap(T& t1, T& t2)
	{
		T temp = t1;
		t1 = t2;
		t2 = temp;
	}

private:
	std::array<T, type_size> m_array;

    size_t m_pop_cursor;

    size_t m_push_cursor;
};

} //namespace light
