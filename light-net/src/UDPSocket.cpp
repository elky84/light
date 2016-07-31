#include "light-net.h"
#include "UDPSocket.h"

#include "P2PCommon.h"


namespace light
{

namespace net
{

UDPSocket::UDPSocket()
: Socket()
, m_receive_buffer(new LinearNormalBuffer(MAX_RECEIVE_BUFFER_SIZE))
{
    create_socket();
}

UDPSocket::~UDPSocket()
{
}

bool UDPSocket::create_socket()
{
	m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(INVALID_SOCKET == m_socket)
	{
		LOG_ERROR(L"socket() error. [%d]", WSAGetLastError());
		return false;
	}
	LOG_DEBUG(L"Success. [%d]", m_socket);
	return true;
}

bool UDPSocket::bind(PORT port)
{
	char pHostName[64];
	gethostname(pHostName, sizeof(pHostName));

	struct addrinfo hints;
	struct addrinfo *result;
	if (getaddrinfo(pHostName, NULL, &hints, &result) != 0)
	{
		return false;
	}

	char ip[20];
	inet_ntop(AF_INET, &result->ai_addr, ip, sizeof(ip));
	for(int i = 0; i < UDP_BIND_TRY_COUNT; i++, port++)
	{
		get_sock_addr(m_sock_addr_in, ip, port);
		int nRet = ::bind(m_socket, (sockaddr*)&m_sock_addr_in, sizeof(m_sock_addr_in));
		if(nRet < 0)
		{
			LOG_DEBUG(L"bind() Failed [%d] [%S:%d] [%d]", nRet, ip, port, WSAGetLastError());
            create_socket(); //바인드 실패하면, 다시 create_socket() 함수 호출 해주어야 함!
			continue;
		}
		LOG_INFO(L"%s Success [%S:%d]", ip, port);
		return true;
	}
	return false;
}

bool UDPSocket::send(const SOCKET &socket, SOCKET_ADDR_IN &sockaddrin, int len, void* data)
{
	return SOCKET_ERROR != sendto(socket, reinterpret_cast<const char*>(data), len, 0, (sockaddr*)&sockaddrin, sizeof(sockaddr));
}

LinearNormalBuffer* UDPSocket::recv(SOCKET_ADDR_IN* sockaddrin)
{
	m_receive_buffer->initialize();
	int nSockaddrIsize = sizeof(SOCKET_ADDR_IN);

	//누구한테 온건지 알아야 되기 때문에, receive 신호 오자마자 recvfrom한다.
	int size = recvfrom(m_socket, m_receive_buffer->get_tail_buffer(), m_receive_buffer->get_remain_length(), 0, (sockaddr*)sockaddrin, &nSockaddrIsize);
	if(SOCKET_ERROR == size)
	{
		if(WSAECONNRESET == WSAGetLastError())
		{
			LOG_DEBUG(L"SOCKET_ERROR [%d] Do you send to invalid adddress?", WSAGetLastError());
			return NULL;
		}

		LOG_INFO(L"SOCKET_ERROR [%d]", WSAGetLastError());
		return NULL;
	}

	if(!m_receive_buffer->jump_tail(size))
	{
		LOG_ERROR(L"!m_receive_buffer->jump_tail(%d) UsingSize [%d]", size, m_receive_buffer->get_length());
		return NULL;
	}

	return m_receive_buffer.get();
}

} // namespace net

} // namespace light