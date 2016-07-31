#pragma once

#include <vector>
#include <map>
#include "spin_lock.h"
#include "UDPSocket.h"
#include "Thread.h"

namespace light
{

namespace net
{

namespace R2P
{

struct Header;

} // namespace R2P

namespace UDP
{

struct Header;

} // namespace UDP

class P2PRelayPeerEx;

class P2PRelay
{
	typedef std::vector<P2PRelayPeerEx*> VECTOR_PEER;

	typedef std::map<int, VECTOR_PEER> MAP_P2PPeerExGroup;

	typedef std::map<int, P2PRelayPeerEx*> MAP_P2PRealyPeerEx;

public:
	P2PRelay();

	~P2PRelay();

	void broadcast(VECTOR_PEER* peers, PacketStream& stream);

	void send(P2PRelayPeerEx* peer, int nReceiverIdx, PacketStream& stream);

	void enter(int group_no, P2PRelayPeerEx* peer);

	void exit(P2PRelayPeerEx* peer);

	inline void lock()
	{
		m_lock.lock();
	}

	inline void unlock()
	{
		m_lock.unlock();
	}

	void on_update();

	virtual void initialize();

	void on_connect(P2PRelayPeerEx* peer);

	void on_disconnect(P2PRelayPeerEx* peer);

private:
	MAP_P2PPeerExGroup m_peer_groups;

	MAP_P2PRealyPeerEx m_p2p_realy_peer_exs;

	spin_lock m_lock;

	UDPSocket m_udp_socket;

    Thread m_thread;
};

} // namespace net

} // namespace light
