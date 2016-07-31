#include "light-net.h"
#include "P2PRelayPeer.h"
#include "P2PRelayCommon.h"
#include "P2PEventSelect.h"

#include "PacketStream.h"

#include "boost/bind.hpp"

namespace light
{

namespace net
{

P2PRelayPeer::P2PRelayPeer(light::weak_raw_ptr<P2PInterface> p2p, PACKET_DISPATCHER& dispatcher)
	: m_p2p(p2p)
{
	m_dispatcher = &dispatcher;

	m_dispatcher->register_func(R2P::AUTH_KEY, boost::bind(&P2PRelayPeer::on_auth_key, this, _1));
	m_dispatcher->register_func(R2P::AUTH_INFORMATION, boost::bind(&P2PRelayPeer::on_auth_information, this, _1));

	m_dispatcher->register_func(R2P::PEERINFORMATION, boost::bind(&P2PRelayPeer::on_peer_information, this, _1));
	m_dispatcher->register_func(R2P::DISCONNECT_PEER, boost::bind(&P2PRelayPeer::on_disconnect_peer, this, _1));
	m_dispatcher->register_func(R2P::PING, boost::bind(&P2PRelayPeer::on_ping, this, _1));
	m_dispatcher->register_func(R2P::RELAY, boost::bind(&P2PRelayPeer::on_relay, this, _1));
}

void P2PRelayPeer::on_connect()
{
	LOG_INFO(L"called");
}

void P2PRelayPeer::on_disconnect()
{
	reuse();
}

void P2PRelayPeer::on_update()
{

}

bool P2PRelayPeer::send(PacketHeader* header)
{
	return Peer::send(header->get_size(), header);
}

bool P2PRelayPeer::on_peer_information(PacketStream& stream)
{
	PeerInformation peerinformation;
	stream >> peerinformation;

	m_p2p->on_recv_peer_infomation(&peerinformation);
	return true;
}

bool P2PRelayPeer::on_disconnect_peer(PacketStream& stream)
{
	R2P::DisconnectPeer disconnectpeer;
	stream >> disconnectpeer;
	m_p2p->remove_peer(disconnectpeer.m_idx);
	return true;
}

bool P2PRelayPeer::on_ping(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);
	return true;
}

bool P2PRelayPeer::on_relay(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);
	PacketHeader header;
	stream >> header;

	m_dispatcher->call(header.command(), stream);
	return true;
}

bool P2PRelayPeer::on_auth_key(PacketStream& stream)
{
	R2P::AuthKey authkey;
	stream >> authkey;

	SOCKET_ADDR_IN sockaddrin;
	get_sock_addr(sockaddrin, get_ip().c_str(), P2P_ECHO_PORT);
	UDPSocket::send(m_p2p->get_socket(), sockaddrin, _countof(authkey.m_szHashKey), authkey.m_szHashKey);
	return true;
}

bool P2PRelayPeer::on_auth_information(PacketStream& stream)
{
	R2P::AuthInformation authInformation;
	stream >> authInformation;
	m_p2p->set_idx(authInformation.m_idx);

	m_p2p->on_complete();
	return true;
}

} // namespace net

} // namespace light
