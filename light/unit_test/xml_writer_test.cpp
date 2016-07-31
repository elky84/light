#include "stdafx.h"
#include "XMLWriter.h"
#include "DirectoryUtil.h"

const std::wstring strPath = L".\\.xml";

TEST(XMLWriter, Write)
{
	light::XMLWriter writer;
	ASSERT_TRUE(writer.open(strPath, L"Xml"));

	for(int i = 0; i < 10; ++i)
	{
		ASSERT_TRUE(writer.into(L"into"));
		{
			ASSERT_TRUE(writer.elem(L"Info"));

			ASSERT_TRUE(writer.attribute(L"Name", L""));
			ASSERT_TRUE(writer.attribute(L"ServerID", L"9389"));

			std::wstringstream strName;
			strName << L"TestScript" << i;

			ASSERT_TRUE(writer.attribute(L"ID", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Password", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Nick", strName.str()));

			ASSERT_TRUE(writer.attribute(L"AuthType", L"2"));
			ASSERT_TRUE(writer.attribute(L"XmlFile", L"Login.xml"));
		}

		{
			ASSERT_TRUE(writer.elem(L"Description"));

			ASSERT_TRUE(writer.attribute(L"Name", L"&&DS>FSDF<,<<.>"));
			ASSERT_TRUE(writer.attribute(L"ServerID", L"9389"));

			std::wstringstream strName;
			strName << L"TestScript" << i;

			ASSERT_TRUE(writer.attribute(L"ID", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Password", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Nick", strName.str()));

			ASSERT_TRUE(writer.attribute(L"AuthType", L"2"));
			ASSERT_TRUE(writer.attribute(L"XmlFile", L"Login.xml"));
		}

		ASSERT_TRUE(writer.out());
	}
}

TEST(XMLWriter, Cleanup)
{
	ASSERT_TRUE( true == light::FileUtil::remove( strPath ) );
}

TEST(XMLWriter, NotUseFile)
{
	light::XMLWriter writer;
	ASSERT_TRUE(writer.into(L"Xml"));
	for(int i = 0; i < 2; ++i)
	{
		ASSERT_TRUE(writer.into(L"into"));
		{
			ASSERT_TRUE(writer.elem(L"Info"));

			ASSERT_TRUE(writer.attribute(L"Name", L""));
			ASSERT_TRUE(writer.attribute(L"ServerID", L"9389"));

			std::wstringstream strName;
			strName << L"TestScript" << i;

			ASSERT_TRUE(writer.attribute(L"ID", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Password", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Nick", strName.str()));

			ASSERT_TRUE(writer.attribute(L"AuthType", L"2"));
			ASSERT_TRUE(writer.attribute(L"XmlFile", L"Login.xml"));
		}

		{
			ASSERT_TRUE(writer.elem(L"Description"));

			ASSERT_TRUE(writer.attribute(L"Name", L"&&DS>FSDF<,<<.>"));
			ASSERT_TRUE(writer.attribute(L"ServerID", L"9389"));

			std::wstringstream strName;
			strName << L"TestScript" << i;

			ASSERT_TRUE(writer.attribute(L"ID", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Password", strName.str()));
			ASSERT_TRUE(writer.attribute(L"Nick", strName.str()));

			ASSERT_TRUE(writer.attribute(L"AutType", L"2"));
			ASSERT_TRUE(writer.attribute(L"XmlFile", L"Login.xml"));
		}

		ASSERT_TRUE(writer.out());
	}

	ASSERT_TRUE(writer.out());

	std::wstring strXML = writer.read_doc();
	ASSERT_TRUE(strXML.empty() == false);
}

const TCHAR* szStr = 
{\
L"<CommandID>\r\n\
<Command ID=\"3\"/>\r\n\
</CommandID>\r\n"};

TEST(XMLWriter, AttributeToUseInteger)
{
	light::XMLWriter writer;
	ASSERT_TRUE(writer.into(L"CommandID"));
	ASSERT_TRUE(writer.elem(L"Command"));
	ASSERT_TRUE(writer.attribute(L"ID", 3));
	ASSERT_TRUE(writer.out());

	std::wstring strXML = writer.read_doc();
	ASSERT_TRUE(strXML == szStr);
}

TEST(XMLWriter, AttributeToUseString)
{
	light::XMLWriter writer;
	ASSERT_TRUE(writer.into(L"CommandID"));
	ASSERT_TRUE(writer.elem(L"Command"));
	ASSERT_TRUE(writer.attribute(L"ID", L"3"));
	ASSERT_TRUE(writer.out());

	std::wstring strXML = writer.read_doc();
	ASSERT_TRUE(strXML == szStr );
}
