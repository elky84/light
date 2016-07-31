#pragma once

#include "Socket.h"

namespace light
{

namespace net
{

class TCPSocket
	: public Socket
{
public:
    TCPSocket();

    virtual ~TCPSocket();

	virtual bool create_socket();
};

} // namespace net

} // namespace light
