#include "light-net.h"
#include "ListenInterface.h"

namespace light
{

namespace net
{

ListenInterface::ListenInterface()
{

}

ListenInterface::~ListenInterface()
{

}

bool ListenInterface::bind(unsigned short port)
{
	m_tcp_socket.get_sock_addr_in()->sin_family = AF_INET;
    m_tcp_socket.get_sock_addr_in()->sin_port = htons(port);
    m_tcp_socket.get_sock_addr_in()->sin_addr.s_addr = htonl(INADDR_ANY); 	//모든 접속을 받겠다

	int nRet = ::bind(m_tcp_socket.get_socket(), (SOCKET_ADDR*)m_tcp_socket.get_sock_addr_in(), sizeof(SOCKET_ADDR_IN));
	if(0 != nRet)
	{
		LOG_ERROR(L"bind() error. [%d]", WSAGetLastError());
		return false;
	}
	return true;
}

bool ListenInterface::listen(int waiting_queue)
{
	int nRet = ::listen(m_tcp_socket.get_socket(), waiting_queue);
	if(0 != nRet)
	{
		LOG_ERROR(L"listen() error. [%d]", WSAGetLastError());
		return false;
	}
	return true;
}

} // namespace net

} // namespace light
