#include "stdafx.h"
#include "BoostPool.h"

static const size_t CHUNK_SIZE = 1024;

TEST( BoostPool, Object_OverSizeTest )
{
	int size = CHUNK_SIZE + 1;

	char* buffer = light::BoostPool<char[CHUNK_SIZE]>::Instance()->allocate(size);
	ASSERT_TRUE(buffer != NULL);

	int allocateSize =  light::BoostPool<char[CHUNK_SIZE]>::Instance()->get_capacity(size);

	ASSERT_TRUE(allocateSize == CHUNK_SIZE * 2);

	light::BoostPool<char[CHUNK_SIZE]>::Instance()->free(buffer, size);
}


TEST( BoostPool, Object_SameSizeTest )
{
	int size = CHUNK_SIZE;

	char* buffer = light::BoostPool<char[CHUNK_SIZE]>::Instance()->allocate(size);
	ASSERT_TRUE(buffer != NULL);

	int allocateSize =  light::BoostPool<char[CHUNK_SIZE]>::Instance()->get_capacity(size);

	ASSERT_TRUE(allocateSize == CHUNK_SIZE);

	light::BoostPool<char[CHUNK_SIZE]>::Instance()->free(buffer, size);
}

TEST( BoostPool, Object_LessSizeTest )
{
	int size = CHUNK_SIZE - 1;

	char* buffer = light::BoostPool<char[CHUNK_SIZE]>::Instance()->allocate(size);
	ASSERT_TRUE(buffer != NULL);

	int allocateSize =  light::BoostPool<char[CHUNK_SIZE]>::Instance()->get_capacity(size);

	ASSERT_TRUE(allocateSize == CHUNK_SIZE);

	light::BoostPool<char[CHUNK_SIZE]>::Instance()->free(buffer, size);
}

