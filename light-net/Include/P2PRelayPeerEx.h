#pragma once

#include <vector>

#include "PeerEx.h"
#include "P2PRelayCommon.h"
#include "P2PRelay.h"
#include "Dispatcher.hpp"

#include "smart_singleton.h"

namespace light
{

namespace net
{

class P2PRelayPeerEx 
	: public PeerEx
	, public smart_singleton<P2PRelay>
{
	typedef std::vector<P2PRelayPeerEx*> VECTOR_PEER;

public:
	P2PRelayPeerEx(const int& idx);

	~P2PRelayPeerEx();

	virtual void on_connect();

	virtual void on_disconnect();

	inline void set_p2p_relay_peers(VECTOR_PEER* peers){m_p2p_relay_peers = peers;}

	inline VECTOR_PEER* get_p2p_relay_peers(){return m_p2p_relay_peers;}

	bool send(PacketStream& stream);

	virtual void on_update();

	inline void set_udp_sock_addr_in(const SOCKET_ADDR_IN& sock_addr_in)
	{
		m_udp_sock_addr_in = sock_addr_in;
	}

	inline const SOCKET_ADDR_IN& get_udp_sock_addr_in()
	{
		return m_udp_sock_addr_in;
	}

	void send_auth_information();

private:
	bool on_enter(PacketStream& stream);

	bool on_exit(PacketStream& stream);

	bool on_ping(PacketStream& stream);

	bool on_relay(PacketStream& stream);

private:
	VECTOR_PEER* m_p2p_relay_peers;

	SOCKET_ADDR_IN m_udp_sock_addr_in;

	PACKET_DISPATCHER m_my_dispatcher;
};

} // namespace net

} // namespace light
