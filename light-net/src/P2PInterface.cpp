#include "light-net.h"
#include "P2PInterface.h"
#include "LinearBuffer.h"
#include <MMSystem.h>

namespace light
{

namespace net
{

P2PInterface::P2PInterface() 
: m_idx(0)
{

}

P2PInterface::~P2PInterface()
{
	clear_peer();
}

void P2PInterface::clear_peer()
{
	m_peers.clear();
}

bool P2PInterface::on_receive()
{
	SOCKET_ADDR_IN sockaddrin;
	LinearNormalBuffer* buffer = m_udp_socket.recv(&sockaddrin);
	if(buffer == NULL) 
        return false;

	PacketHeader* header = (PacketHeader*)buffer->get_head_buffer();
	if(header->get_size() < buffer->get_length())  //헤더가 작으면 장난치는걸로 판단~! 데이터가 쪼금 왔을 수도?
	{
		return false;
	} 

	if(header->command() == UDP::IS_CONNECTED) //접속 요청 패킷이 피어 정보보다 먼저 올 가능성이 있다.
	{
		P2PPeer* peer = get_peer(header->index()); //피어를 찾아보고
		if(peer)
		{
			peer->set_sock_addr_in(sockaddrin); //있으면 주소 갱신
		}
		else
		{
			new_peer(header->index(), sockaddrin); //없으면 피어 생성
		}
	}
	return receive(header);
}

bool P2PInterface::receive(PacketHeader* header)
{
	P2PPeer* peer = get_peer(header->index());
	if(peer == NULL)
	{
		LOG_ERROR(L"Peer not find [%d] [%d]", header->index(), header->command());
		return false;
	}

	return peer->verify(header->get_size(), header) ? true : false;
}

void P2PInterface::on_recv_peer_infomation(PeerInformation* peer_information)
{
	if(get_peer(peer_information->m_idx))
		return;

	new_peer(peer_information->m_idx, peer_information->m_udp_sock_addr_in);
}

P2PPeer* P2PInterface::new_peer(int idx, const SOCKET_ADDR_IN& sockaddrin)
{
	MAP_P2PPeer::iterator it = m_peers.find(idx);
	if(it != m_peers.end())
		return NULL;

	P2PPeer* peer = add_peer(idx, sockaddrin);
    m_peers.insert({ idx, SHARED_PTR_P2P_PEER(peer) });

	char ip[20];
	inet_ntop(AF_INET, (void*)&sockaddrin.sin_addr, ip, sizeof(ip));

	LOG_INFO(L"[%d] [%S:%d]", peer->get_idx(), ip, ntohs(sockaddrin.sin_port));
	peer->try_connect();
	return peer;
}

P2PPeer* P2PInterface::get_peer(int idx)
{
	MAP_P2PPeer::iterator it = m_peers.find(idx);
	if(it == m_peers.end())
		return NULL;

	return it->second.get();
}

void P2PInterface::remove_peer(int idx)
{
	MAP_P2PPeer::iterator it = m_peers.find(idx);
	if(it == m_peers.end())
	{
		LOG_ERROR(L"NotFind Peer [%d]", idx);
		return;
	}

	m_peers.erase(it);

	LOG_INFO(L"[%d]", idx);
}

void P2PInterface::on_update()
{
	for(MAP_P2PPeer::iterator it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		SHARED_PTR_P2P_PEER& refPeer = it->second;
		refPeer->on_update();
	}
}

void P2PInterface::broadcast(PacketStream& stream)
{
	for(MAP_P2PPeer::iterator it = m_peers.begin(); it != m_peers.end();++it)
	{
		SHARED_PTR_P2P_PEER& peer = it->second;
		peer->send(stream);
	}
}

void P2PInterface::send(int idx, PacketStream& stream)
{
	P2PPeer* peer = get_peer(idx);
	if(peer)
	{
		peer->send(stream);
	}
}

bool P2PInterface::on_packet(unsigned int len, void* data)
{
	UNREFERENCED_PARAMETER(len);

	PacketHeader* header = (PacketHeader*)data;
	P2PPeer* peer = get_peer(header->index());
	if(peer == NULL)
	{
		LOG_ERROR(L"Peer not find [%d] [%d]", header->index(), header->command());
		return false;
	}

	peer->on_receive(header->get_size(), header);
	return true;
}

bool P2PInterface::bind(PORT port)
{
	return m_udp_socket.bind(port);
}

} // namespace net

} // namespace light
