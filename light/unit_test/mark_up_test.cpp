#include "stdafx.h"
#include "Markup.h"
#include "DirectoryUtil.h"
#include <fstream>
#include "FileUtil.h"

const std::wstring strPath = light::get_module_path() + _T("data.xml");
std::wstring XML_Doc;

TEST(MarkUp, WriteTest)
{
	CMarkup xml;  

	ASSERT_TRUE(xml.AddElem( _T("ORDER") ));
	ASSERT_TRUE(xml.AddAttrib( _T("type"), _T("B") ));

	ASSERT_TRUE(xml.AddChildElem( _T("ITEM") ));
	ASSERT_TRUE(xml.IntoElem());
	ASSERT_TRUE(xml.AddAttrib( _T("type"), _T("A") ));
	ASSERT_TRUE(xml.AddChildElem( _T("SN"), _T("132487A-J") ));
	ASSERT_TRUE(xml.AddChildElem( _T("NAME"), _T("crank casing") ));
	ASSERT_TRUE(xml.AddChildElem( _T("QTY"), _T("1") ));
	ASSERT_TRUE(xml.OutOfElem());

	ASSERT_TRUE(xml.AddChildElem( _T("ITEM") ));
	ASSERT_TRUE(xml.IntoElem());
	ASSERT_TRUE(xml.AddAttrib( _T("type"), _T("C")));
	ASSERT_TRUE(xml.AddChildElem( _T("SN"), _T("434417F-Y") ));
	ASSERT_TRUE(xml.AddChildElem( _T("NAME"), _T("kully casing") ));
	ASSERT_TRUE(xml.AddChildElem( _T("QTY"), _T("2") ));
	ASSERT_TRUE(xml.OutOfElem() );

	XML_Doc = xml.GetDoc();

    std::wofstream stream(strPath);
    stream << XML_Doc;
    stream.close();
}

TEST(MarkUp, ReadStreamTest)
{
	CMarkup xml;  
	ASSERT_TRUE( xml.SetDoc( XML_Doc ) );

	if( xml.FindElem( _T("ORDER") ) )
	{
		std::wstring strAttr = xml.GetAttrib(_T("type"));
		ASSERT_TRUE( false == strAttr.empty() );
	}

    while (xml.FindChildElem (_T("ITEM")))
	{  
		std::wstring strAttr = xml.GetChildAttrib(_T("type"));
		ASSERT_TRUE( false == strAttr.empty() );

		ASSERT_TRUE(xml.IntoElem() );

		ASSERT_TRUE(xml.FindChildElem( _T("SN") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE(xml.FindChildElem( _T("NAME") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE(xml.FindChildElem( _T("QTY") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE( xml.OutOfElem() );
	} 
}


TEST(MarkUp, ReadTest)
{
	CMarkup xml;  
	ASSERT_TRUE( xml.Load( strPath ) );

	if( xml.FindElem( _T("ORDER") ) )
	{
		std::wstring strAttr = xml.GetAttrib(_T("type"));
		ASSERT_TRUE( false == strAttr.empty() );
	}

	while ( xml.FindChildElem( _T("ITEM") ) )  
	{  
		std::wstring strAttr = xml.GetChildAttrib(_T("type"));
		ASSERT_TRUE( false == strAttr.empty() );

		ASSERT_TRUE(xml.IntoElem() );

		ASSERT_TRUE(xml.FindChildElem( _T("SN") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE(xml.FindChildElem( _T("NAME") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE(xml.FindChildElem( _T("QTY") ) );
		ASSERT_TRUE( false == xml.GetChildData().empty() );

		ASSERT_TRUE( xml.OutOfElem() );
	} 
}

TEST(MarkUp, CleanupTest)
{
	ASSERT_TRUE( true == light::FileUtil::remove( strPath ) );
}
