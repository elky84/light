#include "light.h"
#include "Str.h"

#include <mbstring.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

namespace light
{

std::wstring str::remove(const std::wstring& str, WCHAR ch)
{
	std::wstring result = str;
	for(std::wstring::size_type pos = result.find_first_of(ch); pos != std::wstring::npos; pos = result.find_first_of(ch)) 
	{
		result.erase(result.begin() + pos, result.begin() + pos + 1);
	}
	return result;
}

std::wstring str::replace(const std::wstring& src, const std::wstring& target, const std::wstring& replacement)
{
	std::wstring str = src;
	std::wstring::size_type pos = 0; // where we are now
	size_t found;		// where the found data is

	if (target.size () > 0)  // searching for nothing will cause a loop
	{
		while ((found = str.find (target, pos)) != std::wstring::npos)
		{
			str.replace (found, target.size(), replacement);
			pos = found + replacement.size();
		}
	}
	return str;
}

str::VECTOR_STRING str::split(const std::wstring& src, const WCHAR* delimiter)
{
	VECTOR_STRING strings;
	// split by delimiter
	boost::char_separator<WCHAR> sep(delimiter);
	boost::tokenizer<boost::char_separator<WCHAR>, std::wstring::const_iterator, std::wstring> tokens(src, sep);
	std::copy(tokens.begin(), tokens.end(), std::back_inserter<VECTOR_STRING>(strings));
	// trim
	std::for_each(strings.begin(), strings.end(),
		boost::bind(&boost::trim<std::wstring>, _1, std::locale()));
	return strings;
}

str::TUPLE_STRING str::cut(const std::wstring& src, const WCHAR delimeter)
{
	std::wstring::size_type Point = src.find_first_of(delimeter);
	return boost::make_tuple(src.substr(0, Point), src.substr(Point + 1, std::wstring::npos));
}

bool str::is_digit(const std::wstring& str)
{
	if(str.empty()) 
		return false;

	for(size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '.' || str[i] == '-') 
			continue;

		if (str[i] < '0' || str[i] > '9') 
			return false;
	}
	return true;
}

std::wstring str::to_upper(const std::wstring& src)
{
	std::wstring result = src;
	for (std::wstring::size_type i = 0;i < result.size();i++)
		result[i] = (WCHAR)_totupper(result[i]);

	return result;
}

std::wstring str::to_lower(const std::wstring& src)
{
	std::wstring result = src;
	for (std::wstring::size_type i = 0;i < result.size();i++)
		result[i] = (WCHAR)_totlower(result[i]);

	return result;
}

int str::to_int(const std::wstring &str, int default_value)
{
	int v = default_value;
	wscanf_s(str.c_str(), L"%d", &v);
	return v;
}


double str::to_double(const std::wstring &str, double default_value)
{
	double v = default_value;
    wscanf_s(str.c_str(), L"%lf", &v);
	return v;
}


UINT str::to_uint(const std::wstring &str, UINT default_value)
{
	UINT v = default_value;
    wscanf_s(str.c_str(), L"%u", &v);
	return v;
}

std::wstring& str::replace_all(std::wstring& context, const std::wstring& from, const std::wstring& to)
{
	std::wstring::size_type lookHere = 0;
	std::wstring::size_type foundHere;
	while((foundHere = context.find(from, lookHere)) != std::wstring::npos)
	{
		context.replace(foundHere, from.size(), to);
		lookHere = foundHere + to.size();
	}
	return context;
}


void str::erase_last(std::wstring& str, const std::wstring& delimeter)
{
	std::wstring::size_type at = str.find_last_of(delimeter);
	if(std::wstring::npos != at)
	{
		str.erase(str.begin() + at, str.end());
	}

}

} // namespace light
