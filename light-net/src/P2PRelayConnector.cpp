#include "light-net.h"
#include "P2PRelayConnector.h"

#include "P2PRelayPeer.h"
#include "P2PRelayCommon.h"

#include "PacketStream.h"

namespace light
{

namespace net
{

P2PRelayConnector::P2PRelayConnector(light::weak_raw_ptr<P2PInterface> p2p, PACKET_DISPATCHER& dispatcher)
: ConnectorEventSelect(new P2PRelayPeer(p2p, dispatcher))
{
}

P2PRelayConnector::~P2PRelayConnector()
{
}

void P2PRelayConnector::enter(int group_no)
{
	P2R::lock enter(group_no);
	PacketStream stream(P2R::ENTER);
	stream << enter;

	Post(stream);
}

void P2PRelayConnector::exit()
{
	PacketStream stream(P2R::EXIT);
	Post(stream);
}

void P2PRelayConnector::on_update()
{
	PacketStream stream(P2R::PING);
	Post(stream);
}

void P2PRelayConnector::Post(PacketStream& stream)
{
	send(stream.get_length(), stream.ptr());
}

} // namespace net

} // namespace light
