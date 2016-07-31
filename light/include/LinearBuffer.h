#pragma once

#include "light.h"
#include "BoostPool.h"
#include "Seh.h"

namespace light
{

template <typename SizeType>
class LinearBuffer
{
public:
	typedef SizeType SIZE;

	typedef SizeType Position;

    static const INT UNIT = 1024 * 8;

public:
	LinearBuffer(SIZE buffer_size)
		: m_buffer( BoostPool<char[UNIT]>::Instance()->allocate(buffer_size) )
		, m_buffer_size(buffer_size)
		, m_tail(0)
		, m_head(0)
	{
	}

	LinearBuffer(LinearBuffer* buffer)
		: m_buffer( BoostPool<char[UNIT]>::Instance()->allocate(buffer->get_size() ) )
		, m_buffer_size(buffer->get_size())
		, m_tail(0)
		, m_head(0)
	{
		write(buffer->get_length(), buffer->get_head_buffer());
	}

	LinearBuffer(const SIZE len, const void* ptr, const SIZE len2 = 0, const void* ptr2 = 0)
		: m_buffer( BoostPool<char[UNIT]>::Instance()->allocate(len + len2) )
		, m_buffer_size(len + len2)
		, m_tail(0)
		, m_head(0)
	{
		write(len, ptr, len2, ptr2);
	}

	LinearBuffer(const LinearBuffer& get_buffer)
		: m_buffer( BoostPool<char[UNIT]>::Instance()->allocate( get_buffer.get_size() ) )
		, m_buffer_size(get_buffer.get_size())
		, m_tail(0)
		, m_head(0)
	{
		write(get_buffer.get_length(), get_buffer.get_head_buffer());
	}

    ~LinearBuffer()
    {
        free();
    }

	bool operator =(const LinearBuffer& get_buffer)
	{
		initialize();
		return write(get_buffer.get_length(), get_buffer.get_head_buffer());
	}

    inline void free()
    {
        if (m_buffer != NULL)
        {
            BoostPool<char[UNIT]>::Instance()->free(m_buffer, get_size());
            m_buffer = NULL;
        }
    }

	inline SIZE get_length() const
	{
		return get_tail()-get_head();
	}

	inline SIZE get_remain_length() const 
	{
		return get_size() - get_length();
	}

	inline Position get_head() const 
	{
		return m_head;
	}

	inline Position get_tail() const 
	{
		return m_tail;
	}

	inline SIZE get_size() const 
	{
		return m_buffer_size;
	}

	inline char* get_buffer() const 
	{
		return m_buffer;
	}

	inline char* get_buffer(SIZE at) const
	{
		if(m_buffer_size <= at)
		{
			Seh::RaiseException(__FUNCTION__, m_buffer_size, at);
			return NULL;
		}

		return &m_buffer[at];
	}

	inline char* get_head_buffer() const 
	{
		return get_buffer(get_head());
	}

	inline char* get_tail_buffer() const 
	{
		return get_buffer(get_tail());
	}

	template <typename T>
	inline bool read(T& data)
	{
		if(get_length() < sizeof(T))
		{
			Seh::RaiseException(__FUNCTION__, get_length(), sizeof(T));
			return false;
		}

		data = *(T*)get_head_buffer();
		return jump_head(sizeof(T));
	}

	inline bool read(SIZE len, void* Data)
	{
		if(get_length() < len)
		{
			return false;
		}

		memcpy(Data, get_head_buffer(), len);
		return jump_head(len);
	}

    bool extend(SIZE len);

	bool write(const SIZE len, const void* ptr, const SIZE len2 = 0, const void* ptr2 = 0);
	
	bool jump_head(Position position);

	bool jump_tail(Position position);

	void compress();

    void initialize();

private:
	inline void reset_head()
	{
		m_head = 0;
	}

	inline void reset_tail()
	{
		m_tail = 0;
	}
	
private:
	Position m_head;

	Position m_tail;

	SIZE m_buffer_size;

	char* m_buffer;
}; 

/// 작은 사이즈의 버퍼 타입
typedef LinearBuffer<unsigned short> LinearTinyBuffer;

/// 일반 사이즈의 버퍼 타입
typedef LinearBuffer<unsigned int> LinearNormalBuffer;

} //namespace light

#include "LinearBuffer.inl"
