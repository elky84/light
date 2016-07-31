#pragma once

#include "TCPSocket.h"

namespace light
{

namespace net
{

class ListenInterface
{
protected:
	ListenInterface(); 
	
	virtual ~ListenInterface();

	bool bind(unsigned short port);

	bool listen(int waiting_queue);

protected:
    TCPSocket m_tcp_socket;
};

} // namespace net

} // namespace light
