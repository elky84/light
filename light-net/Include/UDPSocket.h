#pragma once

#include "LinearBuffer.h"
#include "Socket.h"

namespace light
{

namespace net
{

class UDPSocket 
    : public Socket
{
public:
	UDPSocket();

	virtual ~UDPSocket();

	virtual bool create_socket();

	bool bind(PORT port);
	
    static bool send(const SOCKET &socket, SOCKET_ADDR_IN &sockaddrin, int len, void* data);

	LinearNormalBuffer* recv(SOCKET_ADDR_IN* sockaddrin);

private:
	std::unique_ptr<LinearNormalBuffer> m_receive_buffer;
};

} // namespace net

} // namespace light
