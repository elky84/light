#pragma once

#include "P2PEventSelect.h"
#include "weak_raw_ptr.hpp"

namespace light
{

namespace net
{

class P2PRelayConnector;
class P2PInterface;

} // namespace net

} // namespace light

class P2P 
	: public light::net::P2PEventSelect
{
public:
	P2P(light::net::PACKET_DISPATCHER& dispatcher);
	~P2P();

	void set(light::weak_raw_ptr<light::net::P2PRelayConnector> RelayConnector);

private:
	virtual light::net::P2PPeer* add_peer(int idx, const light::net::SOCKET_ADDR_IN& sockaddrin);
	virtual void remove_peer(int idx);

	virtual void on_complete();

private:
    light::weak_raw_ptr<light::net::P2PRelayConnector> m_RelayConnector;

	light::net::PACKET_DISPATCHER& m_dispatcher;
};
