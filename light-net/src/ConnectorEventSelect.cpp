#include "light-net.h"
#include "ConnectorEventSelect.h"
#include "Peer.h"
#include "Tick.h"

namespace light
{
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
namespace net
{

ConnectorEventSelect::ConnectorEventSelect(Peer* peer)
: EventSelectInterface(m_tcp_socket.get_socket())
, IoInterface(peer)
, m_connected(false)
, m_tick(new Tick())
, m_receive_buffer(new LinearNormalBuffer(MAX_RECEIVE_BUFFER_SIZE))
, m_send_buffer(new LinearNormalBuffer(MAX_SEND_BUFFER_SIZE))
{
}

ConnectorEventSelect::~ConnectorEventSelect()
{
}

bool ConnectorEventSelect::disconnect(RESULT_CODE::Type code)
{
	LOG_INFO(L"[%d]", code);
	on_disconnect();
    return true;
}

bool ConnectorEventSelect::connect(const char* ip, PORT port)
{
	if( true == is_connected())
	{
		LOG_INFO(L"already connect server");
		return false;
	}

	int value = 0;
	int length = sizeof(value);
	if(getsockopt(m_tcp_socket.get_socket(), SOL_SOCKET, SO_RCVBUF, (char*)&value ,&length) < 0)
	{
		LOG_ERROR(L"getsockopt() SO_RCVBUF fail");
		return false;
	}

	if(getsockopt(m_tcp_socket.get_socket(), SOL_SOCKET, SO_SNDBUF, (char*)&value ,&length) < 0)
	{
		LOG_ERROR(L"getsockopt() SO_SNDBUF fail");
		return false;
	}

	BOOL bNoDelay = TRUE;
	int nRet = ::setsockopt(m_tcp_socket.get_socket(), IPPROTO_TCP, TCP_NODELAY, (const char FAR* )&bNoDelay, sizeof(bNoDelay));
	if( nRet == SOCKET_ERROR )
	{
		LOG_ERROR(L"setsockopt() Error [%d]", nRet);
		return false;
	}

	int size = MAX_SEND_BUFFER_SIZE;
	setsockopt(m_tcp_socket.get_socket(), SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
	setsockopt(m_tcp_socket.get_socket(), SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));

	get_sock_addr(*m_tcp_socket.get_sock_addr_in(), ip, port);
	m_peer->set_sock_addr_in(*m_tcp_socket.get_sock_addr_in());
	
	if( false == ConnectorInterface::connect(m_tcp_socket.get_socket(), m_tcp_socket.get_sock_addr_in()))
	{
		return false;
	}

	on_connect();
	return true;
}

bool ConnectorEventSelect::on_send()
{
	int nRemainSendSize = m_send_buffer->get_length();
	if (::send(m_tcp_socket.get_socket(), m_send_buffer->get_head_buffer(), nRemainSendSize, 0) != SOCKET_ERROR)
	{
		m_send_buffer->jump_head(nRemainSendSize);
		m_send_buffer->compress();
		return true;
	}
	else 
	{
		if(GetLastError() == WSAEWOULDBLOCK) 
		{
			LOG_INFO(L"GetLastError() == WSAEWOULDBLOCK");
			return true;
		}

		disconnect(RESULT_CODE::SEND_FAILED);
		return false;
	}
}

bool ConnectorEventSelect::on_receive()
{
	if(!is_connected())
		return false;

	int size = recv(m_tcp_socket.get_socket(), m_receive_buffer->get_tail_buffer(), m_receive_buffer->get_remain_length(), 0);
	if(SOCKET_ERROR == size) 
		return false;

	on_receive(size);
	return true;
}

bool ConnectorEventSelect::on_receive(light::net::PACKET_SIZE len)
{
	if( false == m_receive_buffer->jump_tail(len) )
		return false;

	m_tick->reset(); //패킷이 왔으면 어떤 패킷이든간에 오케이~
	while(0 < m_receive_buffer->get_length())
	{
		light::net::PACKET_SIZE nPacketLen = m_peer->verify(m_receive_buffer->get_length(), m_receive_buffer->get_head_buffer());
		if(nPacketLen == 0)
			break;

		push(new LinearNormalBuffer(nPacketLen, m_receive_buffer->get_head_buffer()));
		m_receive_buffer->jump_head(nPacketLen);
	}
	m_receive_buffer->compress();
	return true;
}

bool ConnectorEventSelect::on_packet(light::net::PACKET_SIZE len, void* data)
{
	m_peer->on_receive(len, data); //패킷단위로 불려짐. 걍 피어꺼 읽어!
	return true;
}

void ConnectorEventSelect::on_connect()
{
	m_tick->reset();
    m_connected.exchange(true);
	if(m_peer.get())
		m_peer->on_connect();

	LOG_INFO(L"success.");
	EventSelectInterface::on_connect();
}

void ConnectorEventSelect::on_disconnect()
{
	if(!is_connected())
		return;

    m_connected.exchange(false);

	LOG_INFO(L"%d", WSAGetLastError());
	if(m_peer.get())
		m_peer->on_disconnect();

	m_receive_buffer->initialize();
	m_send_buffer->initialize();

    if (m_tcp_socket.get_socket() != INVALID_SOCKET)
        m_tcp_socket.close_socket();

    m_tcp_socket.create_socket();
	EventSelectInterface::on_disconnect();
}

void ConnectorEventSelect::alive_check()
{
	if(!is_connected())
		return; 

	if(!m_tick->Check(GetTickCount()))
	{
		LOG_INFO(L"Check() False");
		disconnect(RESULT_CODE::ALIVE_CHECK_AS_DETECTED);
	}
}

bool ConnectorEventSelect::send(light::net::PACKET_SIZE len, const void* ptr)
{
	scope_lock<spin_lock> cs(m_lock);
	if(false == is_connected() )
		return false;

	if (m_send_buffer->write(len, ptr))  //버퍼 뒤에 데이터를 추가하자
	{ 
		on_send();	//on_send를 직접 불러주어야만 버퍼에 있는 데이터를 전송한다
		return true;
	}
	else 
	{
		disconnect(RESULT_CODE::SEND_FAILED);
		return false;
	}
}

void ConnectorEventSelect::reuse()
{
}

} // namespace net

} // namespace light
