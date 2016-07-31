#include "light-net.h"
#include "ConnectorInterface.h"

namespace light
{

namespace net
{

ConnectorInterface::ConnectorInterface() 
{
}

ConnectorInterface::~ConnectorInterface()
{

}

bool ConnectorInterface::connect(SOCKET socket, SOCKET_ADDR_IN* sockaddrin)
{
	if( true == is_connected() )
		return false;

	int nRet = ::connect(socket, (SOCKET_ADDR*)sockaddrin, sizeof( SOCKET_ADDR_IN ));
	if (nRet == SOCKET_ERROR) 
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
		{
			LOG_INFO(L"WSAWOULDBLOCK");
			return true;
		}

		char ip[20];
		inet_ntop(AF_INET, &sockaddrin->sin_addr, ip, sizeof(ip));

		LOG_INFO(L"connect failed : %d, %s, %d", WSAGetLastError(), ip, ntohs(sockaddrin->sin_port));
		return false;
	}
	return true;
}

} // namespace net

} // namespace light
