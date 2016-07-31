#pragma once

#include "Util.h"

namespace light
{
	
namespace XML_CONVETER
{

enum TYPE
{
	DENIED,
	BOOL,
	CHAR,
	UCHAR,
	SHORT,
	USHORT,
	LONG,
	ULONG,
	LONGLONG,
	ULONGLONG,
	FLOAT,
	DOUBLE,
	TSTRING,
	NULL_TERMINATED_STRING,
};

struct CONVERT_INFO
{
public:
	TYPE m_type;

	size_t m_length;

	void* m_ptr;

public:
	CONVERT_INFO(const TYPE type, const size_t length, void* ptr)
		: m_type(type)
		, m_length(length)
		, m_ptr(ptr)
	{
	}
};

template <bool> struct Enumerator;

template <> struct Enumerator<true>
{
	static const TYPE c_type_value = DENIED;
};

template <typename T> inline TYPE convert_type(T)        { return Enumerator<std::tr1::is_enum<T>::value>::c_type_value; }
template <> inline TYPE convert_type(char)               { return CHAR; }   
template <> inline TYPE convert_type(unsigned char)      { return UCHAR; }
template <> inline TYPE convert_type(bool)               { return BOOL; }
template <> inline TYPE convert_type(short)              { return SHORT; }
template <> inline TYPE convert_type(unsigned short)     { return USHORT; }
template <> inline TYPE convert_type(int)                { return LONG; }
template <> inline TYPE convert_type(unsigned int)       { return ULONG; }
template <> inline TYPE convert_type(long)               { return LONG; }
template <> inline TYPE convert_type(unsigned long)      { return ULONG; }
template <> inline TYPE convert_type(__int64)            { return LONGLONG; }
template <> inline TYPE convert_type(unsigned __int64)   { return ULONGLONG; }

template <> inline TYPE convert_type(float)              { return FLOAT; }
template <> inline TYPE convert_type(double)             { return DOUBLE; }

template <> inline TYPE convert_type(std::wstring)       { return TSTRING; }
template <> inline TYPE convert_type(const std::wstring&){ return TSTRING; }
template <> inline TYPE convert_type(std::wstring&)      { return TSTRING; }
template <> inline TYPE convert_type(WCHAR*)             { return NULL_TERMINATED_STRING; }
template <> inline TYPE convert_type(const WCHAR*)       { return NULL_TERMINATED_STRING; }
//////////////////////////////////////////////////////////////////////////

static bool to_string(const std::wstring& str, const CONVERT_INFO& info)
{
	std::wstring& dest = cast<std::wstring>(info.m_ptr);
	dest = str;
	return true;
}

static bool to_int(const std::wstring& str, const CONVERT_INFO& info)
{
	int& dest = cast<int>(info.m_ptr);
	dest = _wtoi(str.c_str());
	return true;
}

static bool to_int64(const std::wstring& str, const CONVERT_INFO& info)
{
	__int64& dest = cast<__int64>(info.m_ptr);
	dest = _wtoi64(str.c_str());
	return true;
}

static bool to_short(const std::wstring& str, const CONVERT_INFO& info)
{
	short& dest = cast<short>(info.m_ptr);
	dest = static_cast<short>(_ttoi(str.c_str()));
	return true;
}

static bool to_char(const std::wstring& str, const CONVERT_INFO& info)
{
	char& dest = cast<char>(info.m_ptr);
	dest = static_cast<char>(_ttoi(str.c_str()));
	return true;
}

static bool to_double(const std::wstring& str, const CONVERT_INFO& info)
{
	double& dest = cast<double>(info.m_ptr);
	dest = _ttof(str.c_str());
	return true;
}

static bool to_float(const std::wstring& str, const CONVERT_INFO& info)
{
	float& dest = cast<float>(info.m_ptr);
	dest = static_cast<float>(_ttof(str.c_str()));
	return true;
}

static bool to_null_terminated_string(const std::wstring& str, const CONVERT_INFO& info)
{
	WCHAR* dest = cast_ptr<WCHAR>(info.m_ptr);
	_tcsncpy_s(dest, info.m_length, str.c_str(), _TRUNCATE);
	return true;
}

static bool convert(const std::wstring& str, const CONVERT_INFO& info)
{
	switch(info.m_type)
	{
	case BOOL:
	case CHAR:
	case UCHAR:
		return to_char(str, info);
	case SHORT:
	case USHORT:
		return to_short(str, info);
	case LONG:
	case ULONG:
		return to_int(str, info);
	case LONGLONG:
	case ULONGLONG:
		return to_int64(str, info);
	case FLOAT:
		return to_float(str, info);
	case DOUBLE:
		return to_double(str, info);
	case TSTRING:
		return to_string(str, info);
	case NULL_TERMINATED_STRING:
		return to_null_terminated_string(str, info);
	}
	return false;
}

} //namespace XML_CONVETER

} //namespace light
