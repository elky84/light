#pragma once

namespace light
{

namespace db
{

namespace SQLite
{

class CppType
{
public:
	static std::wstring ToSQLiteType(const std::wstring& cpptype, const DWORD Size)
	{
		if(cpptype == L"float")
		{
			return L"DOUBLE";
		}
		else if(cpptype == L"int")
		{
			return L"INTEGER";
		}
		else
		{
			WCHAR temp[100] = {0, };
			_itow(Size, temp, 10);
			return std::wstring(L"VARCHAR") + temp;
		}
	}

};

} // namespace db

} // namespace SQLite

} // namespace light