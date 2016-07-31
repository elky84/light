#include "light-net.h"
#include "Socket.h"

#pragma comment(lib, "ws2_32.lib")

namespace light
{

namespace net
{

Socket::Socket() 
: m_socket(INVALID_SOCKET)
{
	m_sock_addr_in.sin_family = AF_INET;
	memset(m_sock_addr_in.sin_zero, 0, sizeof(m_sock_addr_in.sin_zero));
}

Socket::~Socket()
{
}

bool Socket::close_socket()
{
	return close_socket(m_socket);
}

bool Socket::close_socket(SOCKET& socket)
{
	if(socket == INVALID_SOCKET)
	{
		LOG_INFO(L"socket == INVALID_SOCKET.");
		return false;
	}

	if(SOCKET_ERROR == ::closesocket(socket))
	{
		LOG_ERROR(L"::closesocket() failed. [%d]", WSAGetLastError());
        socket = INVALID_SOCKET;
		return false;
	}
	
    LOG_DEBUG(L"Success.");
    socket = INVALID_SOCKET;
    return true;
}

} // namespace net

} // namespace light
