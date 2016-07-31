#include "stdafx.h"

#include "Patch.hpp"

static const std::wstring strSrc = _T(".\\Patch\\src.txt");
static const std::wstring strDest = _T(".\\Patch\\dest.txt");
static const std::wstring strPatch = _T("patch.pat");
static const std::wstring strResult = _T("result.txt");

class Patch_Fixture 
	: public testing::Test
{
protected:
	virtual void SetUp(void)
	{
		
	}

	virtual void TearDown(void)
	{
	}

protected:
	light::Patcher m_Patcher;

	light::Packer m_Packer;
};

TEST_F(Patch_Fixture, Test)
{
	ASSERT_TRUE( m_Packer.Create(strSrc, strDest, strPatch) );

	ASSERT_TRUE( m_Patcher.Apply(strSrc, strResult, strPatch) );
}