#pragma once

#include "Peer.h"

class server_peer 
	: public light::net::Peer
{
public:
	server_peer();
	virtual ~server_peer();

	virtual void on_connect();
	virtual void on_disconnect();

	virtual void on_update();

	virtual void _Post(light::net::PACKET_SIZE len, const void* ptr);
	
	INT get_idx()
	{
		return 1;
	}
};
