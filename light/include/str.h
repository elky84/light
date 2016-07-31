#pragma once

#include "str_types.h"

#include <atlbase.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/bind.hpp>

///CHAR, WCHAR, WCHAR 간에 변환하는 매크로
#define WCHAR_TO_CHAR(x) CW2A(x) 

///CHAR를 WCHAR로 변환해주는 매크로
#define CHAR_TO_WCHAR(x) CA2W(x) 

#define STRING_TO_WSTRING(x) std::wstring(CHAR_TO_WCHAR(x.c_str()))

#define WSTRING_TO_STRING(x) std::string(WCHAR_TO_CHAR(x.c_str()))

namespace light
{

class str
{
public:
	/// 스트링 vector의 typedef형
	typedef std::vector<std::wstring> VECTOR_STRING;

	/// 문자열 튜플의 typedef형
	typedef boost::tuple<std::wstring, std::wstring> TUPLE_STRING;

public:
	/// 특정 문자를 문자열에서 제거해주는 메서드
	static std::wstring remove(const std::wstring& str, WCHAR ch);

	///  특정 문자열을 치환해주는 메서드
	static std::wstring replace(const std::wstring& src, const std::wstring& target, const std::wstring& replacement);

	///  문자열을 잘라내는 메소드
	static VECTOR_STRING split(const std::wstring& src, const WCHAR* delimeter);

	///  문자열을 잘라내는 메소드
	static TUPLE_STRING cut(const std::wstring& src, const WCHAR delimeter);

	/// 정수 여부 확인
	static bool is_digit(const std::wstring& str);
	
	static std::wstring to_upper(const std::wstring& src);
	
	static std::wstring to_lower(const std::wstring& src);

	static void erase_last(std::wstring& str, const std::wstring& delimeter);

public:
	static int to_int(const std::wstring &str, int default_value = 0);
	static double to_double(const std::wstring &str, double default_value = 0);
	static UINT to_uint(const std::wstring &str, UINT default_value = 0);

	static std::wstring& replace_all(std::wstring& context, const std::wstring& from, const std::wstring& to);
};

} //namespace light
