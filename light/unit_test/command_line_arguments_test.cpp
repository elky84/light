#include "stdafx.h"
#include "CommandLineArguments.h"

#include "DirectoryUtil.h"

TEST(CommandLineArguments, Test)
{
    using namespace light;
    ASSERT_TRUE(CommandLineArguments::parse(get_module_path() + L" -FILE Test.png -X 256 -Y 256 -WIDTH 512 -HEIGHT 512 -CUT_WIDTH 64 -CUT_HEIGHT 64 -FRAME 100"));
	ASSERT_TRUE(CommandLineArguments::has(L"FILE"));
	ASSERT_EQ(CommandLineArguments::to_str(L"FILE"), L"Test.png");

	ASSERT_TRUE( CommandLineArguments::has(L"X"));
	ASSERT_EQ( CommandLineArguments::to_str(L"X"), L"256");

	ASSERT_TRUE( CommandLineArguments::has(L"Y"));
	ASSERT_EQ( CommandLineArguments::to_str(L"Y"), L"256" );

	ASSERT_TRUE( CommandLineArguments::has(L"WIDTH"));
	ASSERT_EQ( CommandLineArguments::to_str(L"WIDTH"), L"512");

	ASSERT_TRUE( CommandLineArguments::has(L"HEIGHT"));
	ASSERT_EQ( CommandLineArguments::to_str(L"HEIGHT"), L"512");

	ASSERT_TRUE( CommandLineArguments::has(L"CUT_WIDTH"));
	ASSERT_EQ( CommandLineArguments::to_str(L"CUT_WIDTH"), L"64");

	ASSERT_TRUE( CommandLineArguments::has(L"CUT_HEIGHT"));
	ASSERT_EQ( CommandLineArguments::to_str(L"CUT_HEIGHT"), L"64");

	ASSERT_TRUE( CommandLineArguments::has(L"FRAME"));
	ASSERT_EQ( CommandLineArguments::to_str(L"FRAME"), L"100");
}
