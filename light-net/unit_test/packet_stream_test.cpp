#include "stdafx.h"
#include "PacketStream.h"

namespace TEST_COMMAND
{
	enum TYPE
	{
		NONE,
		REQ,
		ACK,
        TEST,
	};
}

const light::net::PACKET_SIZE PACKET_BUFFER_SIZE = 1024 * 4;

const std::wstring test_str = _T("askdjfkla;sdvmklzxmckfja38954klajsdklf;jaksdf");
const BYTE integer_array[] = { 100, 200, 156, 213, 232, 5, 12, 42, 54};
const DWORD data = 1429401849;

TEST(PacketStream, constructor)
{
	light::net::PacketStream WritePacket(TEST_COMMAND::REQ);
	WritePacket.WriteStr(test_str);

	light::net::PacketStream ReadPacket( WritePacket.get_length(), WritePacket.get_buffer<char>() );
	EXPECT_EQ(WritePacket.get_length(), ReadPacket.get_size());

	std::wstring strReadData = ReadPacket.ReadStr();
	EXPECT_EQ(test_str, strReadData.c_str());
}

TEST(PacketStream, method)
{
	light::net::PacketStream WritePacket(TEST_COMMAND::ACK);
	WritePacket.WriteStr(test_str);
	for(size_t i = 0; i < _countof(integer_array); ++i)
		WritePacket.Write1(integer_array[i]);

	WritePacket.Write4(data);

	light::net::PacketStream ReadPacket( WritePacket.get_length(), WritePacket.get_buffer<char>() );
	EXPECT_EQ(WritePacket.get_length(), ReadPacket.get_size());

	std::wstring read = ReadPacket.ReadStr();
	EXPECT_EQ(test_str, read.c_str());

	for(size_t i = 0; i < _countof(integer_array); ++i)
	{
		BYTE nReadData = ReadPacket.Read1();
		EXPECT_EQ(integer_array[i], nReadData);
	}

	EXPECT_EQ(data, ReadPacket.Read4());
}

TEST(PacketStream, body)
{
    light::net::PacketStream stream(TEST_COMMAND::TEST);
    stream.WriteStr(test_str);
    for (size_t i = 0; i < _countof(integer_array); ++i)
        stream.Write1(integer_array[i]);

    stream.Write4(data);

    ASSERT_EQ(stream.body_size(), stream.get_size() - light::net::PacketHeader::header_size());
}
