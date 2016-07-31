#pragma once

namespace light
{

namespace net
{

class ConnectorInterface
{
public:
	ConnectorInterface();

	virtual ~ConnectorInterface();
	
	virtual bool connect(SOCKET socket, SOCKET_ADDR_IN* sockaddrin);

	virtual bool is_connected() = 0;
};

} // namespace net

} // namespace light
