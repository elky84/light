#include "stdafx.h"
#include "P2P.h"
#include "p2p_peer_ex.h"
#include "P2PRelayConnector.h"

P2P::P2P(light::net::PACKET_DISPATCHER& dispatcher)
	: m_dispatcher(dispatcher)
{
	
}

P2P::~P2P()
{

}

void P2P::set(light::weak_raw_ptr<light::net::P2PRelayConnector> RelayConnector)
{
	m_RelayConnector = RelayConnector;
}

light::net::P2PPeer* P2P::add_peer(int idx, const light::net::SOCKET_ADDR_IN& sockaddrin)
{
	light::net::P2PPeer* peer = new p2p_peer_ex(m_RelayConnector, this, m_dispatcher, m_udp_socket.get_socket(), idx, sockaddrin);
	return peer;	
}

void P2P::remove_peer(int idx)
{
	__super::remove_peer(idx);
}

void P2P::on_complete()
{
	m_RelayConnector->enter(1);
}
