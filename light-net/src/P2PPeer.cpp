#include "light-net.h"
#include "P2PPeer.h"

#include "BitOperator.h"

#include "P2PCommon.h"
#include "P2PEventSelect.h"
#include "P2PRelayConnector.h"
#include "P2PRelayCommon.h"
#include "P2PInterface.h"

#include "PacketStream.h"

#include <boost/bind.hpp>

namespace light
{

namespace net
{

P2PPeer::P2PPeer(light::weak_raw_ptr<P2PRelayConnector> relay_connector, light::weak_raw_ptr<P2PInterface> p2p, PACKET_DISPATCHER& dispatcher, const SOCKET &socket, int idx, const SOCKET_ADDR_IN& sockaddrin)
: m_p2p(p2p)
, m_relay_connector(relay_connector)
, m_socket(socket)
, m_idx(idx)
, m_udp_enable(true)
{
	initialize();
	set_sock_addr_in(sockaddrin);

	m_dispatcher = &dispatcher;

	m_dispatcher->register_func(UDP::IS_CONNECTED, boost::bind(&P2PPeer::on_is_connected, this, _1));
	m_dispatcher->register_func(UDP::SUCCESSED, boost::bind(&P2PPeer::on_successed, this, _1));
}

P2PPeer::~P2PPeer()
{
}

void P2PPeer::initialize()
{
	m_udp_enable = false;
	m_act_tick = GetTickCount();
}

void P2PPeer::release()
{
}

void P2PPeer::set_sock_addr_in(const SOCKET_ADDR_IN& sockaddrin)
{
	m_sock_addr_in = sockaddrin;
	__super::set_sock_addr_in(m_sock_addr_in);
}


bool P2PPeer::on_is_connected(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);

	PacketStream ack(UDP::SUCCESSED);
	send(ack);

    LOG_INFO(L"[%s]", get_ip().c_str());
    return true;
}

bool P2PPeer::on_successed(PacketStream& stream)
{
	UNREFERENCED_PARAMETER(stream);

	m_udp_enable = true;
	LOG_INFO(L"[%s]", get_ip().c_str());
	return true;
}

void P2PPeer::try_connect()
{
	PacketStream ack(UDP::IS_CONNECTED);
	udp_send(ack);	
}

bool P2PPeer::relay_send(PacketStream& stream)
{
	stream.Header()->index(m_p2p->get_idx());

	PacketHeader header(P2R::RELAY);
	header.set_size(PacketHeader::header_size() + stream.get_length());
	LinearNormalBuffer buffer((int)sizeof(header), &header, stream.get_length(), stream.ptr());

	return m_relay_connector->send(buffer.get_size(), buffer.get_buffer());
}

bool P2PPeer::udp_send(PacketStream& stream)
{
	stream.Header()->index(m_p2p->get_idx());
	return UDPSocket::send(m_socket, m_sock_addr_in, stream.get_length(), stream.ptr());
}

bool P2PPeer::send(PacketStream& stream)
{
	bool ret = false;
	if(m_udp_enable)
	{
		ret = udp_send(stream);
	}
	else
	{
		ret = relay_send(stream);
	}	

	if(false == ret)
	{
		LOG_ERROR(L"failed");
	}
	return ret;
}

void P2PPeer::on_update()
{
	DWORD curtick = GetTickCount();
	scope_lock<spin_lock> cs(m_lock);

	if( P2P_ACT_TICK < curtick - m_act_tick)
	{
		if(false == m_udp_enable) // UDP연결되어있지 않으면 핑을 날린다.
		{
			try_connect();
		}

		m_act_tick = curtick;
	}
}

} // namespace net

} // namespace light
