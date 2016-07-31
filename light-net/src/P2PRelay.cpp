#include "light-net.h"
#include "P2PRelay.h"

#include "P2PRelayPeerEx.h"
#include "PacketStream.h"

#include <algorithm>

namespace light
{

namespace net
{

P2PRelay::P2PRelay()
    : m_thread(boost::bind(&P2PRelay::on_update, this))
{
    m_thread.start();
}

P2PRelay::~P2PRelay()
{
    m_thread.stop();
}

void P2PRelay::enter(int group_no, P2PRelayPeerEx* peer)
{
	scope_lock<spin_lock> cs(m_lock);
	if(peer->get_p2p_relay_peers())
	{
		exit(peer);
	}

	auto it = m_peer_groups.find(group_no);
	if(it == m_peer_groups.end())
	{
        m_peer_groups.insert({ group_no, VECTOR_PEER() });
		enter(group_no, peer);
	}
	else
	{
		PeerInformation peer_information(peer->get_idx(), peer->get_udp_sock_addr_in());
		PacketStream peersInfo(R2P::PEERINFORMATION);
		VECTOR_PEER* peers = &it->second;

		peersInfo << peer_information;

		for each(auto target in *peers)
		{
			PeerInformation target_peer_information(target->get_idx(), target->get_udp_sock_addr_in());
			PacketStream other_peerInfo(R2P::PEERINFORMATION);
			
			other_peerInfo << target_peer_information;

			peer->send(other_peerInfo);
		}

		peers->push_back(peer);
		broadcast(peers, peersInfo);

		peer->set_p2p_relay_peers(peers);

		LOG_INFO(L"%s [%d] [%d]", __FUNCTIONW__, group_no, peer->get_idx());
	}
}

void P2PRelay::exit(P2PRelayPeerEx* peer)
{
	scope_lock<spin_lock> cs(m_lock);
	auto peers = peer->get_p2p_relay_peers();
	if(peers)
	{
		peers->erase(remove(peers->begin(), peers->end(), peer));

		peer->set_p2p_relay_peers(NULL);

		R2P::DisconnectPeer peerInfo(peer->get_idx());

		PacketStream stream(R2P::DISCONNECT_PEER);
		stream << peerInfo;

		broadcast(peers, stream);
		LOG_INFO(L"[%d]", peer->get_idx());
	}
	else
	{
		LOG_ERROR(L"Error [%d]", peer->get_idx());
	}
}

void P2PRelay::broadcast(VECTOR_PEER* peers, PacketStream& stream)
{
	if(peers == NULL)
		return;

	scope_lock<spin_lock> cs(m_lock);
	for(VECTOR_PEER::iterator it = peers->begin(); it != peers->end(); ++it)
	{
		P2PRelayPeerEx* peer = (*it);
		peer->send(stream);
	}
}

void P2PRelay::send(P2PRelayPeerEx* peer, int nReceiverIdx, PacketStream& stream)
{
	scope_lock<spin_lock> cs(m_lock);
	VECTOR_PEER* peers = peer->get_p2p_relay_peers();
	if(peers == NULL)
		return;

	for(VECTOR_PEER::iterator it = peers->begin(); it != peers->end(); ++it)
	{
		P2PRelayPeerEx* pCheckP2PRelayPeerEx = (*it);
		if(pCheckP2PRelayPeerEx->get_idx() == nReceiverIdx) //받을 사람 찾으면!
		{
			pCheckP2PRelayPeerEx->send(stream);
			return;
		}
	}
}

void P2PRelay::on_update()
{
	SOCKET_ADDR_IN sockaddrin;
	LinearNormalBuffer* pRecvBuffer = m_udp_socket.recv(&sockaddrin);
	if(pRecvBuffer)
	{
		char* szHashKey = (char*)pRecvBuffer->get_buffer();
		int key = atoi(szHashKey);

		char ip[20];
		inet_ntop(AF_INET, &sockaddrin.sin_addr, ip, sizeof(ip));

		MAP_P2PRealyPeerEx::iterator it = m_p2p_realy_peer_exs.find(key);
		if(it == m_p2p_realy_peer_exs.end()) //없으면??
		{
			LOG_DEBUG(L"Received Udp Signal. But, not find P2PRelayPeerEx [%d] [%s, %d]", key, ip, ntohs(sockaddrin.sin_port));
			return;
		}

		P2PRelayPeerEx* peer = it->second;
		peer->set_udp_sock_addr_in(sockaddrin); //주소 설정
		peer->send_auth_information();
		LOG_INFO(L"Received Udp Signal [%d] [%s, %d]", key, ip, ntohs(sockaddrin.sin_port));
	}
}

void P2PRelay::initialize()
{
	if(false == m_udp_socket.bind(light::net::P2P_ECHO_PORT))
	{
		LOG_ERROR(L"%s [%s, %d]", get_local_ip().c_str(), light::net::P2P_ECHO_PORT);
	}
}

void P2PRelay::on_connect(P2PRelayPeerEx* peer)
{
    m_p2p_realy_peer_exs.insert({ peer->get_idx(), peer });
}

void P2PRelay::on_disconnect(P2PRelayPeerEx* peer)
{
	m_p2p_realy_peer_exs.erase(peer->get_idx());	
}

} // namespace net

} // namespace light
