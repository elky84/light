#pragma once

#include "P2PPeer.h"

enum P2P_TEST_COMMAND;

namespace light
{

namespace net
{

class P2PRelayConnector;
class P2PInterface;

} // namespace net

} // namespace light

class p2p_peer_ex 
	: public light::net::P2PPeer
{
public:
	p2p_peer_ex(light::weak_raw_ptr<light::net::P2PRelayConnector> RelayConnector, light::weak_raw_ptr<light::net::P2PInterface> p2p, light::net::PACKET_DISPATCHER& dispatcher, const SOCKET &rSocket, int idx, const light::net::SOCKET_ADDR_IN& sockaddrin);

	virtual ~p2p_peer_ex();

	virtual void on_connect();

	virtual void on_disconnect();

	virtual void on_update();

private:
	bool Chat(light::net::PacketStream& stream);
};
