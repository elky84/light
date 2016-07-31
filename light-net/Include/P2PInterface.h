#pragma once

#include <map>

#include "P2PPeer.h"
#include "UDPSocket.h"
#include "spin_lock.h"

namespace light
{

namespace net
{

class P2PInterface 
{
private:
	typedef std::shared_ptr<P2PPeer> SHARED_PTR_P2P_PEER;

	typedef std::map<int, SHARED_PTR_P2P_PEER > MAP_P2PPeer;

public:
	P2PInterface();

	virtual ~P2PInterface();
	
	virtual void remove_peer(int idx);

	virtual void send(int idx, PacketStream& stream);

	P2PPeer* get_peer(int idx);

	inline void set_idx(int idx)
	{
		m_idx = idx;
	}

	inline int get_idx()
	{
		return m_idx;
	}

	inline SOCKET_ADDR_IN* get_internal_sock_addr_in()
	{
		return m_udp_socket.get_sock_addr_in();
	}

	inline void set_external_sock_addr_in(SOCKET_ADDR_IN* sock_addr_in)
	{
		m_external_sock_addr_In = *sock_addr_in;
	}

	inline unsigned short get_port()
	{
		return ntohs(m_udp_socket.get_sock_addr_in()->sin_port);
	}

	void on_recv_peer_infomation(PeerInformation* peer_information);
	
	bool receive(PacketHeader* header);
	
	void broadcast(PacketStream& stream);

	virtual void on_complete() = 0;
	
	virtual bool on_packet(unsigned int len, void* data);
	
	virtual void on_update();
	
	virtual bool bind(PORT port);

	virtual void on_receive(PacketHeader* header) = 0;

	inline void lock()
    {
        m_lock.lock();
    }

	inline void unlock()
    {
        m_lock.unlock();
    }


    SOCKET get_socket()
    {
        return m_udp_socket.get_socket();
    }

protected:
	virtual bool on_receive();
	
	virtual P2PPeer* add_peer(int idx, const SOCKET_ADDR_IN& sockaddrin) = 0;

	P2PPeer* new_peer(int idx, const SOCKET_ADDR_IN& sockaddrin);

	void clear_peer();

protected:
	MAP_P2PPeer m_peers;

    int m_idx;

	SOCKET_ADDR_IN m_external_sock_addr_In;

	spin_lock m_lock;

    UDPSocket m_udp_socket;
};

} // namespace net

} // namespace light
