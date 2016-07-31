#include "stdafx.h"
#include "p2p_peer_ex.h"
#include "P2PTestCommon.h"

#include "PacketStream.h"

p2p_peer_ex::p2p_peer_ex(light::weak_raw_ptr<light::net::P2PRelayConnector> RelayConnector, light::weak_raw_ptr<light::net::P2PInterface> p2p, light::net::PACKET_DISPATCHER& dispatcher, const SOCKET &rSocket, int idx, const light::net::SOCKET_ADDR_IN& sockaddrin)
: P2PPeer(RelayConnector, p2p, dispatcher, rSocket, idx, sockaddrin)
{
	m_dispatcher->register_func(P2PCHAT, boost::bind(&p2p_peer_ex::Chat, this, _1));
}


p2p_peer_ex::~p2p_peer_ex()
{
    m_dispatcher->unregister(P2PCHAT);
}

bool p2p_peer_ex::Chat(light::net::PacketStream& stream)
{
	P2PChat chat;
	stream >> chat;

	LOG_INFO(L"[%d] [%s]", get_idx(), chat.STR);
	return true;
}

void p2p_peer_ex::on_update()
{
	__super::on_update();

	static int i = 0;

	P2PChat chat;
	wprintf_s(chat.STR, L"%s %d", L"Test", i++);
	
	light::net::PacketStream stream(P2PCHAT);
	stream << chat;

	send(stream);
}

void p2p_peer_ex::on_connect()
{

}

void p2p_peer_ex::on_disconnect()
{

}
