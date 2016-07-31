#include "stdafx.h"
#include "flatbuffers/flatbuffers.h"

#include "packet_generated.h"

TEST(flatbuffers, file_test)
{
    using namespace PACKET;

    flatbuffers::FlatBufferBuilder builder;
    auto name = builder.CreateString("elky");
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

    auto enter = flatbuffers::GetRoot<ENTER>(data);
    ASSERT_EQ(std::string(enter->USER_NAME()->c_str()), "elky");
    ASSERT_EQ(enter->USER_INDEX(), 250);
}

TEST(flatbuffers, memory_test)
{
    using namespace PACKET;

    // 메모리에 데이터를 만들고 읽어 들이기.
    flatbuffers::FlatBufferBuilder builder;
    auto name = builder.CreateString("elky");
    auto object = CreateENTER(builder, builder.CreateString("elky"), 250);
    builder.Finish(object);

    uint8_t* buf = builder.GetBufferPointer();

    auto enter = flatbuffers::GetRoot<ENTER>(buf);
    ASSERT_EQ(std::string(enter->USER_NAME()->c_str()), "elky");
    ASSERT_EQ(enter->USER_INDEX(), 250);

}
