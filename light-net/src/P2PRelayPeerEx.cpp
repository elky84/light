#include "light-net.h"
#include "P2PRelayPeerEx.h"

#include "PacketStream.h"
#include "boost/bind.hpp"

namespace light
{

namespace net
{

P2PRelayPeerEx::P2PRelayPeerEx(const int& idx) 
	: PeerEx(idx)
	, m_p2p_relay_peers(NULL)
{
	m_dispatcher = &m_my_dispatcher;

	m_dispatcher->register_func(P2R::ENTER, boost::bind(&P2PRelayPeerEx::on_enter, this, _1));
	m_dispatcher->register_func(P2R::EXIT, boost::bind(&P2PRelayPeerEx::on_exit, this, _1));
	m_dispatcher->register_func(P2R::PING, boost::bind(&P2PRelayPeerEx::on_ping, this, _1));
	m_dispatcher->register_func(P2R::RELAY, boost::bind(&P2PRelayPeerEx::on_relay, this, _1));
}

P2PRelayPeerEx::~P2PRelayPeerEx()
{
	on_disconnect();
}

void P2PRelayPeerEx::on_connect()
{
	char szHashKey[MAX_AUTH_KEY] = "";

	sprintf_s(szHashKey, _countof(szHashKey), "%d", get_idx());

	R2P::AuthKey key(szHashKey);
	PacketStream stream(R2P::AUTH_KEY);
	stream << key;

	send(stream);

	smart_instance()->on_connect(this);
}

void P2PRelayPeerEx::on_disconnect()
{
	if(m_p2p_relay_peers)
	{
		smart_instance()->exit(this);
		m_p2p_relay_peers = NULL;
	}

	smart_instance()->on_disconnect(this);

	reuse();
}


void P2PRelayPeerEx::on_update()
{
	PacketStream stream(R2P::PING);
	send(stream);
}

bool P2PRelayPeerEx::send(PacketStream& stream)
{
	return Peer::send(stream.get_length(), stream.get_head());
}

void P2PRelayPeerEx::send_auth_information()
{
	R2P::AuthInformation info(get_idx());
	PacketStream stream(R2P::AUTH_INFORMATION);
	stream << info;

	send(stream);
}

bool P2PRelayPeerEx::on_enter(PacketStream& stream)
{
	P2R::lock enter;
	stream >> enter;
	smart_instance()->enter(enter.m_GroupNo, this);
	return true;
}

bool P2PRelayPeerEx::on_exit(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);
	smart_instance()->exit(this);
	return true;
}

bool P2PRelayPeerEx::on_ping(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);
	return true;
}

bool P2PRelayPeerEx::on_relay(PacketStream& stream)
{
    smart_instance()->broadcast(get_p2p_relay_peers(), stream);
    return true;
}

} // namespace net

} // namespace light
