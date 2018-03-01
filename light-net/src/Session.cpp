#include "light-net.h"
#include "Session.h"

#include "PeerEx.h"
#include "IocpInterface.h"
#include "IocpContext.h"
#include "scope_lock.h"
#include "Repository.h"

namespace light
{

namespace net
{

Session::Session(const int idx, IocpInterface* iocp_interface) 
: m_connected(false)
, m_idx(idx)
, IoInterface(NULL)
, m_connect_context(new IocpContext(this, new IocpDelegateConnect))
, m_reuse_context(new IocpContext(this, new IocpDelegateReuse))
, m_disconnecting(false)
, m_iocp_interface(iocp_interface)
{
    auto repository = Repository<STAT, Statistics<int> >::Instance();

    m_stats.insert({ STAT::ON_RECEIVE, repository->get(STAT::ON_RECEIVE) });

    m_stats.insert({ STAT::SEND, repository->get(STAT::SEND) });
    m_stats.insert({ STAT::ON_SEND, repository->get(STAT::ON_SEND) });

    m_stats.insert({ STAT::ON_CONNECT, repository->get(STAT::ON_CONNECT) });
    m_stats.insert({ STAT::CONNECT_FAILED, repository->get(STAT::CONNECT_FAILED) });

    m_stats.insert({ STAT::DISCONNECT, repository->get(STAT::DISCONNECT) });
    m_stats.insert({ STAT::ON_DISCONNECT, repository->get(STAT::ON_DISCONNECT) });
}

Session::~Session()
{
}

bool Session::disconnect(RESULT_CODE::Type code)
{
    LOG_INFO(L"[%d] [%u]", get_idx(), code);
    m_stats[STAT::DISCONNECT]->record(1);

    if (false == m_iocp_interface->disconnect(this, code))
    {
        check_reuse(); // if disconnect failed post check_reuse.
        return false;
    }

    return true;
}

void Session::initialize()
{
    m_connect_context->initialize();
    m_reuse_context->initialize();

    if (m_receive_context.get())
        m_receive_context->initialize();
    else
        m_receive_context.reset(new IocpContext(this, new IocpDelegateReceive));

    if (m_send_context.get())
        m_send_context->initialize();
    else
        m_send_context.reset(new IocpContext(this, new IocpDelegateSend(new LinearNormalBuffer(MAX_SEND_BUFFER_SIZE))));
}

void Session::on_reuse(RESULT_CODE::Type code)
{
    if (code != RESULT_CODE::SUCCESS)
    {
        m_tcp_socket.close_socket();
    }

    m_reuse_context->set_register(false);
    if ( is_register() )
    {
        LOG_INFO(L"[%d] register actions. [%d] [%d] [%d] [%d] [%d]", m_idx, code, m_receive_context->is_register(), m_connect_context->is_register(), m_reuse_context->is_register(), m_send_context->is_register());
        return;
    }

    if (m_iocp_interface)
        m_iocp_interface->on_reuse(this, code);

    m_disconnecting.exchange(false);
    m_connected.exchange(false);

    LOG_INFO(L"[%d] success. [%d]", m_idx, code);
}

bool Session::send(light::net::PACKET_SIZE len, const void* ptr)
{
    scope_lock<spin_lock> lock(m_send_lock);
    if ( m_disconnecting )
        return false;

    if ( false == is_connected() )
        return false;

    if ( m_send_context->get_size() < m_send_context->get_length() + len)
    {
        LOG_ERROR(L"failed write to buffer [%d] [%d] [%d]", get_idx(), m_send_context->get_length(), len);
        disconnect(RESULT_CODE::SEND_BUFFER_WRITE_FAILED);
        return false;
    }

    if (false == m_send_context->write(len, ptr))
    {
        LOG_ERROR(L"failed write to buffer [%d] [%d] [%d]", get_idx(), m_send_context->get_length(), len);
        disconnect(RESULT_CODE::OVERFLOW_SEND_SIZE);
        return false;
    }

    m_stats[STAT::SEND]->record(len);

    if ( m_send_context->is_register())
    {
        return false;
    }
    return send();
}


void Session::set_sock_addr_in(const SOCKET_ADDR_IN* sockaddr)
{
    m_tcp_socket.set_sock_addr_in(sockaddr);
}

SOCKET_ADDR_IN* Session::get_sock_addr_in()
{
    return m_tcp_socket.get_sock_addr_in();
}

std::string Session::get_ip()
{
    char addr4_str[20];
    inet_ntop(AF_INET, &m_tcp_socket.get_sock_addr_in()->sin_addr, addr4_str, sizeof(addr4_str));
    return addr4_str;
}

int Session::get_port()
{
    return ntohs(m_tcp_socket.get_sock_addr_in()->sin_port);
}

bool Session::is_connected()
{
    return m_connected.load();
}

bool Session::send()
{
    m_send_context->set_register(true);
    m_send_context->begin();

    LOG_TRACE(L"try send [%d] [%d]", get_idx(), m_send_context->get_length());
    
    DWORD dwSend = 0;
    int ret = WSASend(get_socket(), m_send_context->get_wsa_buf(), 1, &dwSend, 0, m_send_context.get(), NULL);
    DWORD err = WSAGetLastError();
    if (ret == 0)
    {
        LOG_TRACE(L"immediately return [%d] [%u] [%d]", get_idx(), dwSend, m_send_context->get_length());
        m_send_context->read(dwSend);
        m_send_context->end();
        m_send_context->set_register(false);
        return true;
    }
    else if (ret == SOCKET_ERROR && err == WSA_IO_PENDING)
    {
        LOG_TRACE(L"io_pending. WSA_IO_PENDING [%d] [%u] [%d]", get_idx(), dwSend, m_send_context->get_length());
        return true;
    }
    else
    {
        LOG_ERROR(L"fail [%d] [%d] [%u] [%d]", err, get_idx(), dwSend, m_send_context->get_length());
        m_send_context->set_register(false);
        m_iocp_interface->on_io_failed(this, RESULT_CODE::WSASEND_IO_FAILED);
        return false;
    }
}

void Session::on_send(DWORD sended)
{
    scope_lock<spin_lock> lock(m_send_lock);

    LOG_TRACE(L"[%d] [%u] [%d]", get_idx(), sended, m_send_context->get_length());
    m_send_context->read(sended);
    m_send_context->end();

    m_stats[STAT::ON_SEND]->record(sended);

    m_send_context->set_register(false);
    if (check_reuse())
        return;

    if (m_send_context->get_length()) // remain length..? need register send.
        send();
}

bool Session::check_reuse()
{
    if (is_disconnecting() && false == is_register())
    {
        LOG_INFO(L"register object all returned. to on_reuse [%d]", get_idx());
        on_reuse(RESULT_CODE::DISCONNECTING_COMPLETE);
        return true;
    }

    return false;
}

void Session::on_connect(RESULT_CODE::Type code)
{
    m_connect_context->set_register(false);
    if(m_iocp_interface)
		m_iocp_interface->on_connected(this, code);

	if(m_peer.get())
		m_peer->on_connect();

    m_stats[STAT::ON_CONNECT]->record(1);

	LOG_DEBUG(L"[%d]", get_idx());
    receive();
}

void Session::on_disconnect(RESULT_CODE::Type code)
{
	if(m_iocp_interface)
		m_iocp_interface->on_disconnect(this, code);

    m_stats[STAT::ON_DISCONNECT]->record(1);
}

void Session::disconnect_process()
{
	if(m_peer.get())
	{
		m_peer->on_disconnect();
	}
}


bool Session::receive()
{
    DWORD dwRecv = 0, dwFlags = 0;

    m_receive_context->begin();
    m_receive_context->set_register(true);

    int ret = WSARecv(get_socket(), m_receive_context->get_wsa_buf(), 1, &dwRecv, &dwFlags, m_receive_context.get(), NULL);
    DWORD err = WSAGetLastError();
    if (ret == 0)
    {
        LOG_TRACE(L"immediately return [%d] [%u] [%d]", get_idx(), dwRecv, m_receive_context->get_length());
        m_receive_context->set_register(false);
        return true;
    }
    else if (ret == SOCKET_ERROR && err == WSA_IO_PENDING)
    {
        LOG_TRACE(L"io_pending [%d] [%u] [%d]", get_idx(), dwRecv, m_receive_context->get_length());
        return true;
    }
    else
    {
        LOG_ERROR(L"fail [%d, %u] [%d] [%d]", get_idx(), dwRecv, err, m_receive_context->get_length());
        m_receive_context->set_register(false);
        m_iocp_interface->on_io_failed(this, RESULT_CODE::WSARECV_IO_FAILED);
        return false;
    }
}

bool Session::on_receive(light::net::PACKET_SIZE len)
{
    m_stats[STAT::ON_RECEIVE]->record(len);

    m_receive_context->set_register(false);
    if (check_reuse())
    {
        return false;
    }

    DWORD err = WSAGetLastError();
    if (0 == len)
    {
        if (err == 64 || err == 0 || err == 997)
        {
            LOG_TRACE(L"%d client_close. Err[%d]", get_idx(), err);
            disconnect(RESULT_CODE::CLOSE_ACTION);
            return false;
        }
        else if(err == WSA_IO_PENDING) // 수신이 0이면서, WSA_IO_PENDING 이면 수신 시그널 트리거를 다시 걸어준다.
        {
            receive();
            return true;
        }
		else
		{
			LOG_TRACE(L"%d. Err[%d]", get_idx(), err);
			disconnect(RESULT_CODE::GQCS);
			return false;
		}
    }

	if(false == m_receive_context->write(len))
	{
        LOG_ERROR(L"m_receive_context->write(%u) failed [%d]", len, get_idx());
		disconnect(RESULT_CODE::OVERFLOW_RECEIVE_BUFFER);
		return false;
	}

	while(0 < m_receive_context->get_length())
	{
		PACKET_SIZE parse_length = m_peer->verify(m_receive_context->get_length(), m_receive_context->get_head_buffer());
		if(parse_length == 0) // 0이면 패킷이 아직 덜온것
		{
			break;
		}
		else if (parse_length == INVALID_FILE_SIZE) // 일단 이상한 넘이다 싶으면 더이상 받지 않는다.
		{
            disconnect(RESULT_CODE::INVALID_DATA_ON_VERIFY);
			return false;
		}
		else //다른 크기면 유효한걸로 봄.
		{
            m_peer->on_receive(parse_length, m_receive_context->get_head_buffer());
            m_receive_context->read(parse_length);
		}
	}

    m_receive_context->end();

    if (true == m_receive_context->is_full())
    {
        if (m_receive_context->get_size() < MAX_RECEIVE_BUFFER_SIZE)
        {
            m_receive_context->extend(m_receive_context->get_size() + DEFAULT_RECEIVE_BUFFER_SIZE);
        }
        else
        {
            LOG_TRACE(L"m_receive_context->is_full() failed [%d] [%d]", len, get_idx());
            disconnect(RESULT_CODE::PACKET_BUFFER_FULL);
            return false;
        }
    }

	return receive(); //다음 통지 신호 받고 싶다고 등록
}

void Session::set_peer(Peer* peer)
{
	m_peer.reset(peer);
	if(m_peer.get())
	{
		m_peer->set_io_interface(this);
		m_peer->set_sock_addr_in(*m_tcp_socket.get_sock_addr_in());
	}
}

void Session::clear_peer()
{
	m_peer.reset();
}

void Session::reuse()
{
    if (m_reuse_context->is_register())
    {
        LOG_INFO(L"reusing state. [%d] [%d] [%S:%d]", WSAGetLastError(), get_idx(), get_ip().c_str(), get_port());
        return;
    }

    if( SOCKET_ERROR == ::shutdown(get_socket(), SD_BOTH)) // 송수신 둘다 안하는 중이면, SOCKET_ERROR반환되기도 한다.
    { 
        LOG_TRACE(L"failed shutdown(). [%d] [%d] [%S:%d]", WSAGetLastError(), get_idx(), get_ip().c_str(), get_port());
    }

    m_reuse_context->set_register(true);
    LOG_TRACE(L"try reuse [%d] [%d] [%S:%d]", WSAGetLastError(), get_idx(), get_ip().c_str(), get_port());

    BOOL ret = ::TransmitFile(get_socket(), NULL, 0, 0, m_reuse_context.get(), NULL, TF_DISCONNECT | TF_REUSE_SOCKET);
    if (TRUE == ret)
    {
        LOG_TRACE(L"TransmitFile() immediately success [%d] [%d] [%S:%d]", WSAGetLastError(), get_idx(), get_ip().c_str(), get_port());
        on_reuse(RESULT_CODE::SUCCESS);
		return;
	}

	if (WSAGetLastError() == WSA_IO_PENDING) // WSA_IO_PENDING은 다음 시그널이 도착한다. 즉 지금은 바로 처리안하고 패스해도 됨.
		return;

	LOG_ERROR(L"TransmitFile() Fail [%d] [%d] [%S:%d]", WSAGetLastError(), get_idx(), get_ip().c_str(), get_port());
    on_reuse(RESULT_CODE::TRANSMITFILE_FAILED);
}

void Session::set_connected(bool flag)
{
    m_connected.exchange(flag);
}

bool Session::is_register() const
{
    if (m_receive_context->is_register() ||
        m_connect_context->is_register() ||
        m_reuse_context->is_register() || 
        m_send_context->is_register() )
    {
        LOG_TRACE(L"[%d] register actions. [%d] [%d] [%d] [%d]", get_idx(), m_receive_context->is_register(), m_connect_context->is_register(), m_reuse_context->is_register(), m_send_context->is_register());
        return true;
    }

    return false;
}

void Session::connect_failed(RESULT_CODE::Type code)
{
    m_stats[STAT::CONNECT_FAILED]->record(1);

    m_connect_context->initialize();
	if(m_iocp_interface)
		m_iocp_interface->connect_failed(this, code);
}

} // namespace net

} // namespace light
