#pragma once

#include "flatbuffers\flatbuffers.h"
#include "LinearBuffer.h"

#include "FlatBuffersHelper.h"

namespace light
{

template <typename T>
class FlatBuffersPtr
{
public:
    FlatBuffersPtr(const int len, const void* ptr)
        : m_buffer(new LinearNormalBuffer(len, ptr))
        , m_ptr(NULL)
    {
    }

    FlatBuffersPtr(LinearNormalBuffer& buffer)
        : m_buffer(buffer.release())
        , m_ptr(NULL)
    {
    }

    FlatBuffersPtr()
        : m_ptr(NULL)
    {
    }

    bool parsing()
    {
        m_ptr = light::FlatBuffersHelper::parsing<T>((BYTE*)m_buffer->get_buffer(), (light::net::PACKET_SIZE)m_buffer->get_size());
        if (NULL == m_ptr)
            return false;

        return true;
    }
    
    T* get() const
    {
        return m_ptr;
    }

    T* operator ->() const
    {
        return m_ptr;
    }

    light::LinearNormalBuffer* buffer()
    {
        return m_buffer.get();
    }

private:
    T* m_ptr;

    std::shared_ptr<light::LinearNormalBuffer> m_buffer;
};

} //namespace light