#include "stdafx.h"
#include "Linearbuffer.h"

TEST(Linearbuffer, Constructor)
{
	char szbuffer[512] = "";
	memset(szbuffer, 0xFF, sizeof(szbuffer));

	light::LinearNormalBuffer buffer(sizeof(szbuffer), szbuffer);
	ASSERT_EQ(buffer.get_size(), sizeof(szbuffer));

	ASSERT_TRUE(memcmp(buffer.get_buffer(), szbuffer, buffer.get_size()) == 0);
}

TEST(Linearbuffer, Write)
{
	char szbuffer[512] = "";
	memset(szbuffer, 0xFF, sizeof(szbuffer));

	light::LinearNormalBuffer buffer(sizeof(szbuffer));
	ASSERT_EQ(buffer.get_size(), sizeof(szbuffer));

	ASSERT_TRUE(buffer.write(sizeof(szbuffer), szbuffer));

	ASSERT_TRUE(memcmp(buffer.get_buffer(), szbuffer, buffer.get_size()) == 0);
}

TEST(Linearbuffer, CopyConstructor)
{
	char buffer[512] = "";
	memset(buffer, 0xFF, sizeof(buffer));

	light::LinearNormalBuffer Srcbuffer(sizeof(buffer), buffer);
	ASSERT_EQ(Srcbuffer.get_size(), sizeof(buffer));
	ASSERT_TRUE(memcmp(Srcbuffer.get_buffer(), buffer, Srcbuffer.get_size()) == 0);

	light::LinearNormalBuffer Copybuffer(Srcbuffer);
	ASSERT_EQ(Copybuffer.get_size(), Srcbuffer.get_size());
	ASSERT_TRUE(memcmp(Copybuffer.get_buffer(), Srcbuffer.get_buffer(), Copybuffer.get_size()) == 0);
}
