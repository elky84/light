#pragma once

#include "Peer.h"

class client_peer : public light::net::Peer
{
public:
	client_peer();
	~client_peer();

	void on_update();

	virtual void on_connect();
	virtual void on_disconnect();

	virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);
	virtual light::net::PACKET_SIZE verify(light::net::PACKET_SIZE len, const void* ptr);
};
