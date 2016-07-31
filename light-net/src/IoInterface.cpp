#include "light-net.h"
#include "IoInterface.h"
#include "Peer.h"

namespace light
{

namespace net
{

IoInterface::IoInterface(Peer* peer)
{
	if(peer)
	{
		set_peer(peer);
	}
}

IoInterface::~IoInterface()
{

}

void IoInterface::set_peer(Peer* peer)
{
	m_peer.reset(peer);
	m_peer->set_io_interface(this);
}

} // namespace net

} // namespace light
