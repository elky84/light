#include "stdafx.h"
#include "Stream.h"

const int DataSize = 10;

TEST(Stream, Method)
{
	light::Stream<int> stream(sizeof(int) * DataSize);

	for(size_t i = 0; i < DataSize; ++i)
	{
		EXPECT_TRUE(stream.write(i));
	}

	for(size_t i = 0; i < DataSize; ++i) //실패해야 되는 테스트
	{
		EXPECT_TRUE(false == stream.write(i));
	}

	for(size_t i = 0; i < DataSize; ++i)
	{
		EXPECT_TRUE(stream.read(i));
	}
}

TEST(Stream, Operator)
{
	light::Stream<int> stream(sizeof(int) * DataSize);

	for(size_t i = 0; i < DataSize; ++i)
	{
		EXPECT_TRUE(stream << i);
	}

	for(size_t i = 0; i < DataSize; ++i) //실패해야 되는 테스트
	{
		EXPECT_TRUE(false == stream << i);
	}

	for(size_t i = 0; i < DataSize; ++i)
	{
		EXPECT_TRUE(stream >> i);
	}
}
