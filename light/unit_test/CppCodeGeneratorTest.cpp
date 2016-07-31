#include "stdafx.h"
#include "CppCodeGenerator.h"

TEST(CppCodeGenerator, Test)
{
	light::CppCodeGenerator generator;
	ASSERT_TRUE( generator.Open(_T("Item.cpp"), _T("Item"), _T("")) );
	ASSERT_TRUE( generator.Variable(_T("std::wstring"), _T("Name")) );
	ASSERT_TRUE( generator.Variable(_T("std::wstring"), _T("Author")) );
	ASSERT_TRUE( generator.End() );
}
