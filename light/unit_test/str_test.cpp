#include "stdafx.h"
#include "str.h"

TEST(str, replace)
{
	const TCHAR* src = L"\'\'$@!$\'\'Test1234\'$@!#\'";
	std::wstring result = light::str::replace(src, L"\'", L"&apos;");
	EXPECT_EQ(std::wstring::npos, result.find_first_of(L'\''));
}

TEST(str, Remove)
{
	const TCHAR* src = L"*&*&**&**&*Test1234*&*&*";
	std::wstring result = light::str::remove(src, L'*');
	EXPECT_EQ(std::wstring::npos, result.find_first_of(L'*'));
}

TEST(str, Split)
{
	const TCHAR* src1 = L"Test1";
	const TCHAR* src2 = L"Test2";
	const TCHAR* src3 = L"Test3";
	
	std::wstringstream strCompositeSrc; 
	strCompositeSrc << src1 << L" " << src2 << L" " << src3;

	light::str::VECTOR_STRING strings = light::str::split(strCompositeSrc.str(), L" ");
	ASSERT_EQ(3, strings.size());

	EXPECT_EQ(strings[0], src1);
	EXPECT_EQ(strings[1], src2);
	EXPECT_EQ(strings[2], src3);
}

TEST(str, Cut)
{
	const TCHAR* src1 = L"Test1";
	const TCHAR* src2 = L"Test2";

	std::wstringstream strCompositeSrc; 
	strCompositeSrc << src1 << L":" << src2;

	auto tupleStr = light::str::cut(strCompositeSrc.str(), L':');
	EXPECT_EQ(src1, boost::get<0>(tupleStr));
	EXPECT_EQ(src2, boost::get<1>(tupleStr));
}

const CHAR* szTest = "TestString";
const WCHAR* wszTest = L"TestString";

TEST(str, CHAR_TO_WCHAR)
{
	EXPECT_EQ( wcscmp(CHAR_TO_WCHAR(szTest), wszTest), 0);
}

TEST(str, WCHAR_TO_CHAR)
{
	EXPECT_EQ(strcmp(WCHAR_TO_CHAR(wszTest), szTest), 0);
}

TEST(str, ReplaceAll)
{
	std::wstring test = L"teststring";

	light::str::replace_all(test, L"t", L"z");

	EXPECT_STREQ( L"zeszszring", test.c_str() );
}