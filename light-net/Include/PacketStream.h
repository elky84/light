#pragma once

#include "Stream.h"

namespace light
{

namespace net
{

class PacketStream : private light::Stream<PACKET_SIZE>
{
private:
	static const light::net::PACKET_SIZE DEFAULT_PACKET_BUFFER_SIZE = 1024 * 4;

public:
	PacketStream(const PacketHeader& header, const PACKET_SIZE bufferSize = DEFAULT_PACKET_BUFFER_SIZE)
		: light::Stream<PACKET_SIZE>(bufferSize)
		, m_Header(*get_buffer<PacketHeader>())
	{
		write(header); //헤더 이므로 헤더에 담기는 크기를 증가 시키지 않기 위해, Stream 클래스의 write를 호출해준다.
	}

	PacketStream(PACKET_ID command, PACKET_INDEX index = 0, const PACKET_SIZE bufferSize = DEFAULT_PACKET_BUFFER_SIZE)
		: light::Stream<PACKET_SIZE>(bufferSize)
		, m_Header(*get_buffer<PacketHeader>())
	{
		PacketHeader header(command, PacketHeader::header_size(), index);
		write(header); //헤더 이므로 헤더에 담기는 크기를 증가 시키지 않기 위해, Stream 클래스의 write를 호출해준다.
	}

	virtual ~PacketStream()
	{

	}

	PacketStream(PACKET_SIZE size, const void* data)
		: light::Stream<PACKET_SIZE>(size)
		, m_Header(*get_buffer<PacketHeader>())
	{
		if( false == set(size, data) )
		{
			LOG_ERROR(L"[size:%d, get_length:%d, get_size:%d]", size, get_length(), get_size());
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

public:
	BYTE Read1()
	{
		BYTE Data;
		if( false == read(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}

		return Data;
	}

	WORD Read2()
	{
		WORD Data;
		if( false == read(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}

		return Data;
	}

	DWORD32 Read4()
	{
		DWORD32 Data;
		if( false == read(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}

		return Data;
	}

	DWORD64 Read8()
	{
		DWORD64 Data;
		if( false == read(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}

		return Data;
	}

	std::wstring ReadStr()
	{
		std::wstring str;
		if( false == read(str) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}

		return str;
	}

public:

	void Write1(BYTE Data)
	{
		if( false == write(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

	void Write2(WORD Data)
	{
		if( false == write(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

	void Write4(DWORD32 Data)
	{
		if( false == write(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

	void Write8(DWORD64 Data)
	{
		if( false == write(Data) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

	void WriteStr(std::wstring str)
	{
		if( false == write(str) )
		{
			RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
		}
	}

public:
	inline PacketHeader* Header()
	{
		return get_buffer<PacketHeader>();
	}

public: //override

	PACKET_SIZE get_size() const 
	{
		return light::Stream<PACKET_SIZE>::get_size();
	}

	PACKET_SIZE get_length() const 
	{
		return light::Stream<PACKET_SIZE>::get_length();
	}

	PACKET_SIZE body_size() const 
	{
		return light::Stream<PACKET_SIZE>::get_size() - PacketHeader::header_size();
	}

	template <typename T>
	T* get_buffer()
	{
		return light::Stream<PACKET_SIZE>::get_buffer<T>();
	}

	BYTE* get_head()
	{
		return light::Stream<PACKET_SIZE>::get_head_buffer<BYTE>();
	}

	BYTE* ptr()
	{
		return light::Stream<PACKET_SIZE>::get_buffer<BYTE>();
	}


	BYTE* body()
	{
		return light::Stream<PACKET_SIZE>::get_buffer<BYTE>(PacketHeader::header_size());
	}

	template <typename T>
	PacketStream& operator >> (T& Data)
	{
		read(Data);
		return *this;
	}

	template <typename T>
	PacketStream& operator << (T& Data)
	{
		write(Data);
		return *this;
	}

	template <typename T>
	bool read(T& data)
	{
		if( false == light::Stream<PACKET_SIZE>::read(data))
		{
			return false;
		}

		m_Header.set_size(m_Header.get_size() - sizeof(T));
		return true;
	}

	template <typename T>
	bool write(T& data)
	{
		if( false == light::Stream<PACKET_SIZE>::write(data))
		{
			return false;
		}

		m_Header.set_size( m_Header.get_size() + sizeof(T) );
		return true;
	}

	bool read(std::wstring& str)
	{
		WORD len;
		if(false == read<WORD>(len))
		{
			return false;
		}

		if( m_Header.get_size() < len)
		{
			return false;
		}

		str.resize(len + 1);

		if( false == light::Stream<PACKET_SIZE>::read(len, &str.at(0)))
		{
			return false;
		}

		m_Header.set_size(m_Header.get_size() - len);
		return true;
	}

	/// 일부러 함수 만들어만 둠. 이거 쓰면 컴파일 에러
	bool read(const std::wstring&);

	/// 일부러 함수 만들어만 둠. 이거 쓰면 컴파일 에러
	bool read(const TCHAR*);

	/// 일부러 함수만 만들어만 둠. 이거 쓰면 컴파일 에러
	bool read(TCHAR*);

	bool write(const TCHAR* szString)
	{
		WORD len = (WORD)_tcslen(szString);
		if(false == write<WORD>(len))
		{
			return false;
		}

		if( false == write(len, szString))
		{
			return false;
		}

		return true;
	}

	bool write(TCHAR* szString)
	{
		return write(reinterpret_cast<const TCHAR*>(szString));
	}

	bool write(std::wstring& str)
	{
		return write(reinterpret_cast<const std::wstring&>(str));
	}

	bool write(const std::wstring& str)
	{
		WORD len = (WORD)str.length() * sizeof(TCHAR);
		if(false == write<WORD>(len))
		{
			return false;
		}

		if( false == write(len, str.c_str()))
		{
			return false;
		}

		return true;
	}

	inline bool write(const PACKET_SIZE length, const void* ptr)
	{
		if( false == light::Stream<PACKET_SIZE>::write(length, ptr) )
		{
			return false;
		}

		m_Header.set_size( m_Header.get_size() + length );
		return true;
	}

	bool set(const PACKET_SIZE len, const void* ptr)
	{
		if( false == light::Stream<PACKET_SIZE>::write(len, ptr) )
		{
			return false;
		}

		Stream<PACKET_SIZE>::read(m_Header); //헤더 만큼 읽어와야~~
		return true;
	}

private:
	/// 헤더 참조자.
	PacketHeader& m_Header;
};

} // namespace net

} // namespace light
