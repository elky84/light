#include "stdafx.h"
#include "XMLParser.h"

TEST(XMLParser, Read)
{

    const WCHAR* strXML =
    { \
        L"<CommandFactor>\
	        <map MapID='5'>\
		        <Event EventID='0' Tag = '킹오브파이터' Name = '쿄'>\
			        <Round>3</Round>\
			        </Event>\
		        <Test EventID='1' Tag = '데드오어얼라이브' Name = '카스미'>\
			        <Round>9</Round>\
			        </Test>\
		        <Event EventID='1' Tag = '마블슈퍼히어로즈' Name = '캡틴아메리카'>\
			        <Round No=\'3\'>9</Round>\
		        </Event>\
		        <Event EventID='2' Tag = '다크스토커즈' Name = '드미트리'>\
			        <Round>9</Round>\
		        </Event>\
	        </map>\
	        <map MapID='1'>\
		        <Event EventID='0' Tag = '아랑전설' Name = '테리'>\
			        <Round>9</Round>\
		        </Event>\
		        <Event EventID='1' Tag = '용호의권' Name = '료'>\
			        <Round>9</Round>\
			        </Event>\
	        </map>\
	        <map MapID='2'>\
		        <Event EventID='0' Tag = '스트리트 파이터' Name = '류'>\
			        <Round>3</Round>\
		        </Event>\
		        <Test EventID='1' Tag = '철권' Name = '카즈야'>>\
			        <Round>9</Round>\
		        </Test>\
	        </map>\
        </CommandFactor>"
    };

	light::XMLParser xml;
	ASSERT_TRUE( xml.read_doc(strXML) );

	ASSERT_TRUE( xml.execute(L"/CommandFactor") );

	ASSERT_TRUE( xml.bind_elem(L"map") );

	short nMapID = 0;
	ASSERT_TRUE(xml.bind_attrib(L"MapID", nMapID));
	while( xml.next() )
	{	
		ASSERT_TRUE( xml.bind_elem(L"Event") );

		char nEventID = 0;
		ASSERT_TRUE(xml.bind_attrib(L"EventID", nEventID));

		std::wstring strTag;
		ASSERT_TRUE(xml.bind_attrib(L"Tag", strTag));

		static const int NameLength = 4;
		TCHAR szName[NameLength + 1] = {0, };
		ASSERT_TRUE(xml.bind_attrib(L"Name", szName, NameLength));

//		{
//			// 정의되지 않은 타입 넘기는 테스트.
//			struct Test
//			{
//				int n;
//			};
//
//			Test test;
//			ASSERT_TRUE(xml.bind_attrib(L"Test"), test)); // 컴파일 에러
//		}

//		{
//			/// TCHAR형 배열 사이즈 없이 넘기는 상황
//			WCHAR szBuffer[6] = {0, };
//			ASSERT_TRUE(xml.bind_attrib(L"Test"), szBuffer)); // 컴파일 에러
//		}

		while( xml.next() )
		{
			ASSERT_TRUE( xml.bind_elem(L"Round") );
			while( xml.next() )
			{
				const std::wstring strRound = xml.get_elem();
				ASSERT_TRUE( strRound.empty() == false );
			}

			ASSERT_TRUE( xml.bind_elem(L"Rate") );
			while( xml.next() )
			{
				const std::wstring strRate = xml.get_elem();
				ASSERT_TRUE( strRate.empty() == false );
			}
		}
	}

	ASSERT_TRUE( strXML == xml.read_doc() );
}


TEST(XMLParser, Simple)
{
    const WCHAR* szSimpleXML = { 
        L"<CommandFactor>\
	    <Shutdown>5</Shutdown>\
	    <Commander>Elky</Commander>\
        </CommandFactor>" 
    };

	light::XMLParser xml;
	ASSERT_TRUE( xml.read_doc(szSimpleXML) );

	ASSERT_TRUE( xml.execute(L"/CommandFactor") );

	DWORD ShutdownValue = 0;
	ASSERT_TRUE( xml.get_elem(L"Shutdown", ShutdownValue));

	std::wstring Commander;
	ASSERT_TRUE( xml.get_elem(L"Commander", Commander));

	ASSERT_TRUE( szSimpleXML == xml.read_doc() );
}


TEST(XMLParser, LogTest)
{
    const WCHAR* szLogXML =
    { \
        L"<LOG REG_DATE='2013-12-07 11:17:37'>\
	<USER INDEX='10001' LEVEL='8'>\
		<CHARACTER ITEMID='1000' INDEX='4927866'>\
			<PARTS SLOT_NO='0' ITEMID='136324096'/>\
			<PARTS SLOT_NO='2' ITEMID='136331375'/>\
			<PARTS SLOT_NO='4' ITEMID='136347697'/>\
			<PARTS SLOT_NO='5' ITEMID='136355866'/>\
			<PARTS SLOT_NO='8' ITEMID='136381440'/>\
			<PARTS SLOT_NO='16' ITEMID='136445980'/>\
			<PARTS SLOT_NO='19' ITEMID='136471552'/>\
		</CHARACTER>\
		<BUFF />\
	</USER>\
	\
	<SERVER INDEX='97001' PROPERTY='0'>\
		<CHANNEL INDEX='0' TYPE='1' PROPERTY='0'>\
			<STAGE NO='3' MAP='11' MAP_LEVEL='0' ROUND='2' PLAYER_COUNT='1'>\
				<GAME START_DATE='2011-12-07 11:16:04' END_DATE='2011-12-07 11:17:37' MONEY='77' EXP='0' SCORE='4' ROUND='2'> \
					<ROUND NO='1' SCORE='2' NAME='SUCCESS'/>\
					<ROUND NO='2' SCORE='1' NAME='FAIL'/>\
				</GAME>\
			</STAGE>\
		</CHANNEL>\
	</SERVER>\
</LOG>"
    };

	light::XMLParser xml;
	ASSERT_TRUE( xml.read_doc(szLogXML) );

	ASSERT_TRUE( xml.execute(L"/LOG/SERVER/CHANNEL/STAGE") );

	ASSERT_TRUE( xml.bind_elem(L"GAME") );

	int MONEY = 0;
	ASSERT_TRUE( xml.bind_attrib(L"MONEY", MONEY) );

	int EXP = 0;
	ASSERT_TRUE( xml.bind_attrib(L"EXP", EXP) );

	ASSERT_TRUE( xml.next() );

	ASSERT_TRUE( szLogXML == xml.read_doc() );
}

TEST(XMLParser, Same_depth_Test)
{
    const WCHAR* szTestXML =
    { \
        L"<Test>\
        <thread_count>200</thread_count>\
        <connection_count>200</connection_count>\
        <test_count>1000000</test_count>\
        <pool-size \
        min='20' \
        max = '50' />\
        <connection_1 \
        host-name='ip+port' \
        host-id = 'id' \
        host-password = 'password' \
        host-schema = 'schema' \
        databasename = 'name' \
        default-timeout = '3s'/> \
        <connection_2 \
        host-name = 'ip+port' \
        host-id = 'id' \
        host-password = 'password' \
        host-schema = 'schema' \
        databasename = 'name' \
        default-timeout = '3s'/> \
        </Test>"
    };

    light::XMLParser parser;
    ASSERT_TRUE(parser.read_doc(szTestXML));

    parser.execute(L"/Test");

    unsigned short thread_count = 0;
    parser.get_elem(L"thread_count", thread_count);

    unsigned short connection_count = 0;
    parser.get_elem(L"connection_count", connection_count);

    int test_count = 0;
    parser.get_elem(L"test_count", test_count);

    parser.bind_elem(L"pool-size");
    unsigned short pool_min = 0;
    parser.bind_attrib(L"min", pool_min);

    unsigned short pool_max = 0;
    parser.bind_attrib(L"max", pool_max);

    ASSERT_TRUE(parser.next());

    parser.out();

    parser.bind_elem(L"connection_1");

    std::wstring host_name;
    parser.bind_attrib(L"host-name", host_name);

    std::wstring host_id;
    parser.bind_attrib(L"host-id", host_id);

    std::wstring host_password;
    parser.bind_attrib(L"host-password", host_password);

    std::wstring host_schema;
    parser.bind_attrib(L"host-schema", host_schema);

    std::wstring databasename;
    parser.bind_attrib(L"databasename", databasename);

    unsigned short timeout = 0;
    parser.bind_attrib(L"default-timeout", timeout);

    ASSERT_TRUE(parser.next());

    parser.out();

    parser.bind_elem(L"connection_2");

    std::wstring host_name_2;
    parser.bind_attrib(L"host-name", host_name_2);

    std::wstring host_id_2;
    parser.bind_attrib(L"host-id", host_id_2);

    std::wstring host_password_2;
    parser.bind_attrib(L"host-password", host_password_2);

    std::wstring host_schema_2;
    parser.bind_attrib(L"host-schema", host_schema_2);

    std::wstring databasename_2;
    parser.bind_attrib(L"databasename", databasename_2);

    unsigned short timeout_2 = 0;
    parser.bind_attrib(L"default-timeout", timeout_2);

    ASSERT_TRUE(parser.next());
}
