#pragma once

#include "P2PCommon.h"
#include "P2PInterface.h"
#include "EventSelectInterface.h"

namespace light
{

namespace net
{

class Tick;
class P2PPeer;

class P2PEventSelect 
    : public P2PInterface
    , public EventSelectInterface
{
public:
	P2PEventSelect();

	virtual ~P2PEventSelect();

	virtual bool on_packet(light::net::PACKET_SIZE len, void* data);

	virtual bool on_receive();

	virtual bool on_send();

	virtual bool bind(PORT port);
	
	virtual void on_receive(PacketHeader* header);
};

} // namespace net

} // namespace light
