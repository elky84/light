#pragma once

#include "ConnectorEventSelect.h"
#include "weak_raw_ptr.hpp"

namespace light
{

namespace net
{

class P2PInterface;

class P2PRelayConnector 
	: public ConnectorEventSelect
{
public:
	P2PRelayConnector(light::weak_raw_ptr<P2PInterface> p2p, PACKET_DISPATCHER& dispatcher);

	virtual ~P2PRelayConnector();

	void enter(int group_no);

	void exit();

	void on_update();

	void Post(PacketStream& stream);
};

} // namespace net

} // namespace light
