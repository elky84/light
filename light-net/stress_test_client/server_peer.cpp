#include "stdafx.h"
#include "server_peer.h"

#include "packet_generator.h"

server_peer::server_peer(int idx, ACTION_TYPE& action)
    : m_idx(idx)
    , m_Action(action)
{
    m_packet_generator.reset(new packet_generator);

    m_buffer.reset(new light::LinearNormalBuffer(light::net::MAX_SEND_BUFFER_SIZE) );
}

server_peer::~server_peer()
{

}

void server_peer::on_connect()
{
	LOG_INFO(L"[%d]", m_idx);

	if(m_Action == ACTION_TYPE::TRY_CONNECT_DISCONNECT)
		disconnect(light::net::RESULT_CODE::CLOSE_ACTION);
}

void server_peer::on_disconnect()
{
    LOG_INFO(L"[%d]", m_idx);

    reuse();
}

void server_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(ptr);
    LOG_INFO(L"[%d] len[%d]", m_idx, len);
}


void server_peer::on_update()
{
	if(m_Action == ACTION_TYPE::SEND_RANDOM_PACKET)
	{
        if (true == m_packet_generator->random(m_buffer.get()))
        {
            send(m_buffer->get_length(), m_buffer->get_head_buffer());
            m_buffer->initialize();
        }
	}
    else if (m_Action == ACTION_TYPE::SEND_HEAVY_PACKET)
    {
        if (true == m_packet_generator->heavy(m_buffer.get()))
        {
            send(m_buffer->get_length(), m_buffer->get_head_buffer());
            m_buffer->initialize();
        }
    }
    else if (m_Action == ACTION_TYPE::SEND_LIGHT_PACKET)
    {
        if (true == m_packet_generator->light(m_buffer.get()))
        {
            send(m_buffer->get_length(), m_buffer->get_head_buffer());
            m_buffer->initialize();
        }
    }
}

light::net::PACKET_SIZE server_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(ptr);
    return len;
}
