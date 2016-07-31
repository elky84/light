#include "stdafx.h"
#include "FlatBuffersHelper.h"

#include "packet_generated.h"

TEST(FlatBuffersHelper, file_test)
{
    using namespace PACKET;

    flatbuffers::FlatBufferBuilder builder;
    auto object = CreateENTER(builder, builder.CreateString("elky"), 250);
    builder.Finish(object);

    FILE* fp = fopen("elky.dat", "wb");
    uint8_t* buf = builder.GetBufferPointer();
    fwrite(buf, builder.GetSize(), 1, fp);
    fclose(fp);

    FILE* readingFile = fopen("elky.dat", "rb");
    fseek(readingFile, 0L, SEEK_END);
    int length = ftell(readingFile);
    fseek(readingFile, 0L, SEEK_SET);
    char *data = new char[length];
    fread(data, sizeof(char), length, readingFile);
    fclose(readingFile);

    auto enter = light::FlatBuffersHelper::parsing<PACKET::ENTER>(buf, builder.GetSize());
    ASSERT_EQ(std::string(enter->USER_NAME()->c_str()), "elky");
    ASSERT_EQ(enter->USER_INDEX(), 250);
}

TEST(FlatBuffersHelper, memory_test)
{
    using namespace PACKET;

    // 메모리에 데이터를 만들고 읽어 들이기.
    flatbuffers::FlatBufferBuilder builder;
    auto object = CreateENTER(builder, builder.CreateString("elky"), 250);
    builder.Finish(object);

    uint8_t* buf = builder.GetBufferPointer();

    auto enter = light::FlatBuffersHelper::parsing<PACKET::ENTER>(buf, builder.GetSize());
    ASSERT_EQ(std::string(enter->USER_NAME()->c_str()), "elky");
    ASSERT_EQ(enter->USER_INDEX(), 250);
}


