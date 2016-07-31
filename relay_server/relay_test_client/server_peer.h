#pragma once

#include "Peer.h"
#include "LinearBuffer.h"

class server_peer
    : public light::net::Peer
{
public:
    server_peer(int idx, ACTION_TYPE & action);

    virtual void on_connect();

    virtual void on_disconnect();

    virtual void on_update();

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);

    virtual light::net::PACKET_SIZE verify(light::net::PACKET_SIZE len, const void* ptr);

private:
    ACTION_TYPE& m_Action;

    int m_idx;

    std::unique_ptr<light::LinearNormalBuffer> m_buffer;
};
