#include "stdafx.h"
#include "MemoryUtil.hpp"

TEST(MemoryUtil, ApplyAllHeap)
{
#ifdef _DEBUG
	EXPECT_FALSE(light::LFH::ApplyAllHeap());
#else // _DEBUG
	EXPECT_TRUE(light::LFH::ApplyAllHeap());
#endif //_DEBUG
}

TEST(MemoryUtil, ApplyCrtHeap)
{
#ifdef _DEBUG
	EXPECT_FALSE(light::LFH::ApplyCrtHeap());
#else // _DEBUG
	EXPECT_TRUE(light::LFH::ApplyCrtHeap());
#endif //_DEBUG
}

TEST(MemoryUtil, ApplyProcessHeap)
{
#ifdef _DEBUG
	EXPECT_FALSE(light::LFH::ApplyProcessHeap());
#else // _DEBUG
	EXPECT_TRUE(light::LFH::ApplyProcessHeap());
#endif //_DEBUG
}