#include "light-net.h"
#include "TCPSocket.h"
#include <MSTcpIP.h>

namespace light
{

namespace net
{

TCPSocket::TCPSocket()
: Socket()
{
    create_socket();
}

TCPSocket::~TCPSocket()
{

}

bool TCPSocket::create_socket()
{
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_socket)
	{
		LOG_ERROR(L"socket() error. [%d]", WSAGetLastError());
		return false;
	}

	int zero = 0;
	if( SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&zero, sizeof(zero)) )
	{	  	   
		LOG_ERROR(L"setsockopt() SO_SNDBUF error. [%d]", WSAGetLastError());
	}

	if( SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&zero, sizeof(zero)) )
	{	  	   
		LOG_ERROR(L"setsockopt() SO_RCVBUF error. [%d]", WSAGetLastError());
	}

	// GPG Study SIO_KEEPALIVE_VALS를 테스트하는 방법?? : http://www.gpgstudy.com/forum/viewtopic.php?p=83373
	// Microsoft Windows 2000 TCP/IP 구현 정보 : http://www.microsoft.com/korea/TechNet/network/tcpip2k.mspx
	// MSDN WSAIoctl Function : http://msdn.microsoft.com/en-us/library/ms741621.aspx
	DWORD dwBytesReturned = 0;
	tcp_keepalive	keepalive = { 1, 60000, 2000 };

	if(SOCKET_ERROR == WSAIoctl(m_socket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(keepalive), NULL, 0, &dwBytesReturned, NULL, NULL))
	{
		LOG_ERROR(L"WSAIoctl() SIO_KEEPALIVE_VALS error. [%d]", WSAGetLastError());
		return false;
	}

	return true;
}

} // namespace net

} // namespace light
