#pragma once

#include "Peer.h"
#include "LinearBuffer.h"

class packet_generator;

class server_peer 
	: public light::net::Peer
{
public:
	server_peer(int idx, ACTION_TYPE & action);
    virtual ~server_peer();

	virtual void on_connect();

	virtual void on_disconnect();

	virtual void on_update();

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);

    virtual light::net::PACKET_SIZE verify(light::net::PACKET_SIZE len, const void* ptr);

private:
	std::unique_ptr<packet_generator> m_packet_generator;

	ACTION_TYPE& m_Action;

    int m_idx;

    std::unique_ptr<light::LinearNormalBuffer> m_buffer;
};
