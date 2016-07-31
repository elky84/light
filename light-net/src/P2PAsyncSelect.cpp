#include "light-net.h"
#include "P2PAsyncSelect.h"
#include "P2PPeer.h"
#include <MMSystem.h>


namespace light
{

namespace net
{

P2PAsyncSelect::P2PAsyncSelect() 
: P2PInterface()
, AsyncSelectInterface(m_udp_socket.get_socket())
{
	 
}

P2PAsyncSelect::~P2PAsyncSelect()
{
}

void P2PAsyncSelect::on_receive(PacketHeader* header)
{
	P2PInterface::on_packet(header->get_size(), header);
}

} // namespace net

} // namespace light
