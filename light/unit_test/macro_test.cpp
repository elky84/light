#include "stdafx.h"
#include "Macro.h"

TEST(Macro, Redefinition)
{
	CLASS_DEFINITION(Test);
	//CLASS_DEFINITION(Test); // 중복 정의하면 컴파일 에러남.
}

TEST(Macro, StatckCheck)
{
	STATIC_CHECK(sizeof(DWORD) <= sizeof(int), logical_test); //사이즈 검사 테스트. typedef 한 자료형들에 대한 검사를 미리 해놓으면 좋을 것이다.
}

TEST(Macro, Assert)
{
	ASSERT_MSG(0 < 1, "0 < 1 is true."); //ASSERT가 MSG 포함해서 뜨는지 확인점...
}

TEST(Macro, WhileLimit)
{
	int n = 0;
	while_limit(1000, n < 1000)
	{
		++n;
	}

	EXPECT_EQ(n, 1000);
}

enum TEST_ENUM
{
	TESTENUM_NONE = -1,
	TESTENUM_SUCCESS = 0,
};

TEST(Macro, ToString)
{
	EXPECT_TRUE(_tcscmp(_T("TESTENUM_SUCCESS"), TO_STRING(TESTENUM_SUCCESS)) == 0);
}
