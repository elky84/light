#pragma once

#include "flatbuffers\flatbuffers.h"
#include "Str.h"

namespace light
{

class FlatBuffersHelper
{
public:
    static std::string to_string(flatbuffers::String* str)
    {
        if (str == NULL)
            return std::string();

        return str->str();
    }

    static flatbuffers::Offset<flatbuffers::String> to_string(flatbuffers::FlatBufferBuilder& builder, wchar_t* str)
    {
        return builder.CreateString(WCHAR_TO_CHAR(str));
    }

    static flatbuffers::Offset<flatbuffers::String> to_string(flatbuffers::FlatBufferBuilder& builder, char* str)
    {
        return builder.CreateString(str);
    }

    static flatbuffers::Offset<flatbuffers::String> to_string(flatbuffers::FlatBufferBuilder& builder, const std::wstring& str)
    {
        return builder.CreateString(WSTRING_TO_STRING(str));
    }

    static flatbuffers::Offset<flatbuffers::String> to_string(flatbuffers::FlatBufferBuilder& builder, const std::string& str)
    {
        return builder.CreateString(str);
    }

    template <typename T>
    static T* parsing(const BYTE* data, int size)
    {
        flatbuffers::Verifier verifier(data, size);
        if (false == verifier.VerifyBuffer<T>())
            return NULL;

        T* buffer = const_cast<T*>(flatbuffers::GetRoot<T>(data));
        if (false == buffer->Verify(verifier))
            return NULL;

        return buffer;
    }

    template <typename STREAM, typename OBJ>
    static void write(STREAM& stream, flatbuffers::FlatBufferBuilder& builder, OBJ& object)
    {
        builder.Finish(object);
        stream.write((light::net::PACKET_SIZE)builder.GetSize(), builder.GetBufferPointer());
    }

};

} //namespace light