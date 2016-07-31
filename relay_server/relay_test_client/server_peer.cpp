#include "stdafx.h"
#include "server_peer.h"

server_peer::server_peer(int idx, ACTION_TYPE& action)
    : m_idx(idx)
    , m_Action(action)
{
    m_buffer.reset(new light::LinearNormalBuffer(light::net::MAX_SEND_BUFFER_SIZE));
}

void server_peer::on_connect()
{
    LOG_INFO(L"[%d]", m_idx);
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
}

light::net::PACKET_SIZE server_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(ptr);
    return len;
}
