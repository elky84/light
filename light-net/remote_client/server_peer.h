#pragma once

#include "Peer.h"

class server_peer : public light::net::Peer
{
public:
	server_peer();
	~server_peer();

	virtual void on_update();

	virtual void on_connect();
	virtual void on_disconnect();

	virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);
	virtual light::net::PACKET_SIZE verify(light::net::PACKET_SIZE len, const void* ptr);
};
