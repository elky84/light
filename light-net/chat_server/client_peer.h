#pragma once

#include "PeerEx.h"

class client_peer
	: public light::net::PeerEx
{
public:
	client_peer(const int& idx);
	~client_peer();

	void on_update();

	virtual void on_connect();
	virtual void on_disconnect();

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);

    virtual light::net::PACKET_SIZE verify(light::net::PACKET_SIZE len, const void* ptr);
};
