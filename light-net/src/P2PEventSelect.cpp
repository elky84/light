#include "light-net.h"
#include "P2PEventSelect.h"
#include "LinearBuffer.h"
#include "P2PPeer.h"

namespace light
{

namespace net
{

P2PEventSelect::P2PEventSelect()
: P2PInterface()
, EventSelectInterface(m_udp_socket.get_socket())
{
}

P2PEventSelect::~P2PEventSelect()
{
	on_disconnect();
}

bool P2PEventSelect::on_packet(light::net::PACKET_SIZE len, void* ptr)
{
	return P2PInterface::on_packet(len, ptr);
}

bool P2PEventSelect::on_receive()
{
	return P2PInterface::on_receive();
}

bool P2PEventSelect::on_send()
{
	return true;
}

bool P2PEventSelect::bind(PORT port)
{
	if( true == m_udp_socket.bind(port))
	{
		on_connect(); //성공했을때 등록해야, bind가 성공한 socket을 EventSelect에 사용한다.
		return true;
	}
	return false;
}

void P2PEventSelect::on_receive(PacketHeader* header)
{
	EventSelectInterface::push(new LinearNormalBuffer(header->get_size(), header));
}

} // namespace net

} // namespace light
