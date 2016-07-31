#include "stdafx.h"
#include "BufferPool.h"

TEST( BufferPool, Object_OverSizeTest )
{
	int size = light::BufferPool::CHUNK_SIZE + 1;

	light::Buffer buffer(size);
	ASSERT_TRUE(buffer.Ptr() != NULL);
	ASSERT_TRUE(buffer.Size() == size);
}

TEST( BufferPool, Object_SameSizeTest )
{
	int size = light::BufferPool::CHUNK_SIZE;

	light::Buffer buffer(size);
	ASSERT_TRUE(buffer.Ptr() != NULL);

	ASSERT_TRUE(buffer.Size() == size);
}

TEST( BufferPool, Object_LessSizeTest )
{
	int size = light::BufferPool::CHUNK_SIZE - 1;

	light::Buffer buffer(size);
	ASSERT_TRUE(buffer.Ptr() != NULL);
	ASSERT_TRUE(buffer.Size() == size);
}