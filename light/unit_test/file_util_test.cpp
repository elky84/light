#include "stdafx.h"
#include "FileUtil.h"

#include "DirectoryUtil.h"

static const std::wstring file_name = _T("Test.bin");
static const std::wstring copy_file_name = _T("Test_Copy.bin");

TEST(FileUtil, Open_And_Close) 
{
	FILE* fp = light::FileUtil::open(file_name, _T("at"));
	ASSERT_TRUE( fp != NULL );
	ASSERT_TRUE( light::FileUtil::close(fp) );
}

TEST(FileUtil, CreateTime) 
{
	time_t time;
	ASSERT_TRUE( light::FileUtil::create_time(file_name, time) );
}

TEST(FileUtil, ModifyTime) 
{
	time_t time;
	ASSERT_TRUE( light::FileUtil::modify_time(file_name, time) );
}

TEST(FileUtil, AccessTime) 
{
	time_t time;
	ASSERT_TRUE( light::FileUtil::access_time(file_name, time) );
}

TEST(FileUtil, Copy) 
{
	ASSERT_TRUE( light::FileUtil::copy(file_name, copy_file_name) );
	ASSERT_TRUE( light::FileUtil::is_exist(copy_file_name) );
}

TEST(FileUtil, Delete) 
{
	ASSERT_TRUE( light::FileUtil::remove(copy_file_name) );
	ASSERT_TRUE( false == light::FileUtil::is_exist(copy_file_name) ); // 없어야 성공
}

TEST(FileUtil, Move) 
{
	ASSERT_TRUE( light::FileUtil::move(file_name, copy_file_name) );
	ASSERT_TRUE( false == light::FileUtil::is_exist(file_name) ); // 없어야 성공
	ASSERT_TRUE( light::FileUtil::is_exist(copy_file_name) );
}

TEST(FileUtil, Rename) 
{
	ASSERT_TRUE( light::FileUtil::rename(copy_file_name, file_name) );
	ASSERT_TRUE( light::FileUtil::is_exist(file_name) );
	ASSERT_TRUE( false == light::FileUtil::is_exist(copy_file_name) ); // 없어야 성공
}