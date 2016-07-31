#pragma once

#include "P2PCommon.h"

#include <list>
#include <map>

#include <memory>
#include "Peer.h"

#include "LinearBuffer.h"
#include "spin_lock.h"



namespace light
{

namespace net
{

class P2PInterface;
class P2PRelayConnector;

const int P2P_ACT_TICK = 1000;

class PacketStream;

class P2PPeer
	: public Peer
{
public:
	P2PPeer(light::weak_raw_ptr<P2PRelayConnector> relay_connector, light::weak_raw_ptr<P2PInterface> p2p, PACKET_DISPATCHER& dispatcher, const SOCKET &socket, int idx, const SOCKET_ADDR_IN& sockaddrin);

	virtual ~P2PPeer();

	inline int get_idx()
	{
		return m_idx;
	}
	
	inline SOCKET_ADDR_IN* get_sock_addr_in()
	{
		return &m_sock_addr_in;
	}
	
	void set_sock_addr_in(const SOCKET_ADDR_IN& sockaddrin);

	bool send(PacketStream& stream);

	virtual void on_update(); 

	inline void lock()
	{
		m_lock.lock();
	}

	inline void unlock()
	{
		m_lock.unlock();
	}

	void try_connect();

protected:
	virtual void initialize();

	virtual void release();

private: //전송 기능 묶음

	bool udp_send(PacketStream& stream);

	bool relay_send(PacketStream& stream);

private:
	bool on_is_connected(PacketStream& stream);

	bool on_successed(PacketStream& stream);

private:
	int m_idx;

	bool m_udp_enable;

	SOCKET_ADDR_IN m_sock_addr_in;

	const SOCKET &m_socket;

	spin_lock m_lock;

protected:
    light::weak_raw_ptr<P2PInterface> m_p2p;

    light::weak_raw_ptr<P2PRelayConnector> m_relay_connector;

	DWORD m_act_tick;
};

} // namespace net

} // namespace light
