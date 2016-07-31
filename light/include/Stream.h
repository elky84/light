#pragma once

#include "LinearBuffer.h"

namespace light
{

template <typename SizeType>
class Stream
{
public:
	Stream(SizeType size)
		: m_buffer(size)
	{

	}

	virtual ~Stream()
	{

	}

	template <typename T>
	bool operator >>(T& Data)
	{
		return read(Data);
	}

	template <typename T>
	bool operator <<(T& Data)
	{
		return write(Data);
	}

	template <typename T>
	bool read(T& data)
	{
		if(false == m_buffer.read(data))
		{
			LOG_ERROR(L"[%d] [%d + %d = %d]", sizeof(T), m_buffer.get_length(), m_buffer.get_remain_length(), m_buffer.get_size());
			return false;
		}
		return true;
	}

	template <typename T>
	bool write(T& data)
	{
		if( false == m_buffer.write(sizeof(T), &data))
		{
			LOG_ERROR(L"[%d] [%d + %d = %d]", sizeof(T), m_buffer.get_length(), m_buffer.get_remain_length(), m_buffer.get_size());
			return false;
		}
		return true;		
	}

	inline bool write(const SizeType len, const void* ptr)
	{
		return m_buffer.write(len, ptr);
	}

	inline bool read(const SizeType len, void* ptr)
	{
		return m_buffer.read(len, ptr);
	}

public:
	SizeType get_size() const 
	{
		return m_buffer.get_size();
	}

	SizeType get_length() const 
	{
		return m_buffer.get_length();
	}

	template <typename T>
	T* get_buffer(SizeType at = 0)
	{
		return reinterpret_cast<T*>(m_buffer.get_buffer(at));
	}

protected:
	template <typename T>
	T* get_head_buffer()
	{
		return reinterpret_cast<T*>(m_buffer.get_head_buffer());
	}

private:
	light::LinearBuffer<SizeType> m_buffer;
};

} //namespace light
