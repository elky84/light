#include "stdafx.h"

#include "FileStream.h"
#include "DirectoryUtil.h"

#include <fstream>
#include <iomanip>

TEST(FileStream, In)
{
	char InBuffer[2500] = "";
	char ch = 'A';

	for(int i = 0; i < _countof(InBuffer) - 1; ++i)
	{
		InBuffer[i] = ch++;
		if('Z' == ch)
		{
			ch = 'A';
		}
	}

	light::FileOutStream instream;
	ASSERT_TRUE(instream.Open(_T("FileInTest.cfg")));

	ASSERT_TRUE(instream.Write(_countof(InBuffer), InBuffer));
	ASSERT_TRUE( 0 < instream.Size() ? true : false );
	instream.Close();
}

TEST(FileStream, Out)
{
	light::FileInStream outstream;
	ASSERT_TRUE(outstream.Open(_T("FileInTest.cfg")));
	ASSERT_TRUE( 0 < outstream.Size() ? true : false ); //읽은 파일 크기
	outstream.Close();
}

TEST(FileStream, IO)
{
	char InBuffer[2500] = "";
	char OutBuffer[2500] = "";
	char ch = 'A';

	for(int i = 0; i < _countof(InBuffer) - 1; ++i)
	{
		InBuffer[i] = ch++;
		if('Z' == ch)
		{
			ch = 'A';
		}
	}

	light::FileInOutStream iostream;
	bool bRet = iostream.Open(_T("FileIoTest.cfg"));
	ASSERT_TRUE(bRet);
	if(false == bRet)
		return;

	iostream.Write(_countof(InBuffer), InBuffer);
	size_t nIOSize = iostream.Size();
	ASSERT_TRUE( 0 < nIOSize ? true : false);

	char tempbuffer[4000] = "";
	for(int i = 0; i < _countof(tempbuffer) - 1; ++i)
	{
		tempbuffer[i] = '1';
	}

	std::ofstream file;
	file.open(_T("FileStreamTest.cfg"));
	ASSERT_FALSE(file.fail());
	file << tempbuffer;
	ASSERT_FALSE(file.fail());

	ASSERT_TRUE(iostream.Read(_countof(OutBuffer), OutBuffer));

	ASSERT_EQ(iostream.Size() + _countof(OutBuffer), nIOSize);

	ASSERT_TRUE(_stricmp(InBuffer, OutBuffer) == 0);

	iostream.Close();
}
