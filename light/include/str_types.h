#pragma once

#include <iostream>
#include <tchar.h>
#include <string>

namespace std
{

typedef basic_string<WCHAR> wstring;
typedef basic_ostream<WCHAR> wostream;
typedef basic_istream<WCHAR> wistream;

typedef basic_stringstream<WCHAR> wstringstream;
typedef basic_ostringstream<WCHAR> wostringstream;
typedef basic_istringstream<WCHAR> wistringstream;

typedef basic_ios<WCHAR, char_traits<WCHAR> > wios;
typedef basic_streambuf<WCHAR, char_traits<WCHAR> > wstreambuf;

typedef basic_stringbuf<WCHAR, char_traits<WCHAR>, allocator<WCHAR> > wstringbuf;

typedef basic_filebuf<WCHAR, char_traits<WCHAR> > wfilebuf;
typedef basic_ifstream<WCHAR, char_traits<WCHAR> > wifstream;
typedef basic_ofstream<WCHAR, char_traits<WCHAR> > wofstream;
typedef basic_fstream<WCHAR, char_traits<WCHAR> > wfstream;

static const wstring tab = L"\t";

} // namespace std