#include "stdafx.h"
#include "BitOperator.h"

TEST(BitOperator, bit_sum)
{
	int a = 100, b = 200;
	ASSERT_EQ(light::bit_sum(a, b), a | b);
}

TEST(BitOperator, bit_xor)
{
	int a = 100, b = 200;
	ASSERT_EQ(light::bit_xor(a, b), a ^ b);
}

TEST(BitOperator, bit_and)
{
	int a = 100, b = 200;
	ASSERT_EQ(light::bit_and(a, b), a & b);
}

TEST(BitOperator, bit_compare)
{
	int a = 0x00001100, b = 0x00000100;
	ASSERT_TRUE(light::bit_compare(a, b));
}

TEST(BitOperator, bit_state)
{
	int a = 0x00000010;
	ASSERT_TRUE(light::bit_state(a, 4));
}

TEST(BitOperator, bit_on)
{
	int a = 0x00000000, b = 0x00000010;
	light::bit_on(a, 4);
	ASSERT_EQ(a, b);
}

TEST(BitOperator, bit_off) 
{
	int a = 0x00000110, b = 0x00000010;
	light::bit_off(a, 8);
	ASSERT_EQ(a, b);
}
