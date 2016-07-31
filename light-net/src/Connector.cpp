#include "light-net.h"
#include "Connector.h"

#include "Peer.h"
#include "Session.h"

#include "SessionManager.h"
#include "IocpThreadManager.h"
#include "IocpContext.h"
#include "Repository.h"

namespace light
{

namespace net
{

std::vector<HANDLE> Connector::m_iocps;

#pragma comment(lib,"mswsock.lib") 

Connector::Connector(Peer* peer)
    : m_lpfn_connect_ex(NULL)
{
	m_session_manager->alloc(1);
	m_session = m_session_manager->pop();
	m_peer = peer;
	m_session->set_peer(peer);

    auto repository = Repository<STAT, Statistics<int> >::Instance();
    m_stats.insert({ STAT::CONNECT, repository->get(STAT::CONNECT) });
}

Connector::~Connector()
{
    if(m_session_manager.get())
        m_session_manager->push(m_session.get());
}

void Connector::create_thread(int thread_count)
{
    IocpInterface::initialize();

    for (int n = 0; n < thread_count; ++n)
    {
        HANDLE iocp = create_iocp_handle();
        m_iocp_thread_manager->add(iocp);
        m_iocps.push_back(iocp);
	}
}

void Connector::delete_thread()
{
    IocpInterface::release();
}

void Connector::on_update()
{
	if( is_connected() )
	{
		m_peer->on_update();
	}
	else
	{
		connect();
	}
}

void Connector::disconnect(RESULT_CODE::Type code)
{
	disconnect(m_session.get(), code);
}

bool Connector::disconnect(Session* session, RESULT_CODE::Type code)
{
    if ( false == session->is_connected())
    {
        LOG_TRACE(L"not connected [%d] [%u]", get_idx(), code);
        return false;
    }

    if( false == session->disconnect() )
	{
        LOG_TRACE(L"already disconnecting [%d] [%u]", get_idx(), code);
        return false;
	}

    LOG_DEBUG(L"[%d] [%u]", get_idx(), code);
    session->on_disconnect(code);
    return true;
}

bool Connector::close_socket()
{
	return m_session->close_socket();
}


bool Connector::connect()
{
    if (m_session->get_connect_context()->is_register())
        return false;

    if ( m_session->is_disconnecting() ||
        is_connected())
        return false;

    if (m_session->get_socket() == INVALID_SOCKET)
        set();

    LOG_TRACE(L"try connect  [%d] [%d]", get_idx(), is_connected());

    m_session->get_connect_context()->set_register(true);

    m_stats[STAT::CONNECT]->record(1);

    BOOL ret = m_lpfn_connect_ex(m_session->get_socket(), (struct sockaddr*)m_session->get_sock_addr_in(), sizeof(struct sockaddr), NULL, 0, NULL, m_session->get_connect_context());
    DWORD err = WSAGetLastError();
    if (FALSE == ret && err != WSA_IO_PENDING)
    {
        LOG_ERROR(L"failed %d [%d] [%d]", err, get_idx(), is_connected());
        if (WSAEISCONN == err || err == WSAENOTSOCK)
        {
            connect_failed(m_session.get(), RESULT_CODE::CONNECT_FAILED_SOCKET_ERROR);
        }
        else
        {
            connect_failed(m_session.get(), RESULT_CODE::CONNECT_FAILED_IMMEDIATE);
        }
        return false;
    }
    return true;
}

void Connector::on_reuse(Session* session, RESULT_CODE::Type code)
{
    LOG_TRACE(L"[%d] [%d]", session->get_idx(), code);
}

void Connector::on_disconnect(Session* session, RESULT_CODE::Type code)
{
	if(is_connected())
	{
		session->disconnect_process();
	}

	if( m_disconnect_callback )
	{
		m_disconnect_callback(code);
	}
}

bool Connector::set()
{
    if ( INVALID_SOCKET != m_session->get_socket() )
    {
        m_session->close_socket();
    }

	if( false == m_session->create_socket())
	{
        LOG_ERROR(L"socket create failed. [%d] [%d]", m_session->get_idx(), WSAGetLastError());
		return false;
	}

	DWORD dwBytes = 0;
	GUID GuidConnectEx = WSAID_CONNECTEX;
	if (SOCKET_ERROR  == ::WSAIoctl( m_session->get_socket(), SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidConnectEx, sizeof(GuidConnectEx), &m_lpfn_connect_ex, sizeof(m_lpfn_connect_ex) , &dwBytes, NULL, NULL))
	{
		LOG_ERROR(L"WSAIoctl() fail [%d] [%d]", WSAGetLastError(), get_idx());
		close_socket();
		return false;
	}

    std::string ip = get_local_ip();
	// 소켓주소구조체를 구성해서 bind작업을 한다. 주소구조체 구성시에는 Local주소중 하나를 사용하고, Protocol family만 지정해주면 된다.
	SOCKET_ADDR_IN localAddr;
	::ZeroMemory(&localAddr, sizeof(SOCKET_ADDR_IN)); 
	localAddr.sin_family = AF_INET;   
	inet_pton(AF_INET, ip.c_str(), &localAddr.sin_addr.s_addr);

	if( SOCKET_ERROR == ::bind( m_session->get_socket(), (SOCKET_ADDR*)&localAddr,sizeof(localAddr) ) )
	{
		LOG_ERROR(L"bind() error. [%d] [%S] [%d]", WSAGetLastError(), ip.c_str(), get_idx());
		close_socket();
		return false;
	}

	HANDLE handle_iocp = CreateIoCompletionPort((HANDLE)m_session->get_socket(), m_iocps[get_idx() % m_iocps.size()], (ULONG_PTR)m_session.get(), 0);
	if (INVALID_HANDLE_VALUE == handle_iocp)
	{
		LOG_ERROR(L"CreateIoCompletionPort fail : [%d] [%d] [%d]", WSAGetLastError(), m_session->get_socket(), get_idx());
		close_socket();
		return false;
	}

	m_session->set_iocp_interface(this);
	return true;
}

bool Connector::set(const std::wstring& IP, PORT port)
{
	int len = WideCharToMultiByte(CP_ACP, 0, &IP[0], -1, NULL, 0, NULL, NULL);
	std::string IP_mbcs(len, 0);
	WideCharToMultiByte(CP_ACP, 0, &IP[0], -1, &IP_mbcs[0], len, NULL, NULL);

	SOCKADDR_IN sockaddrin;
	get_sock_addr(sockaddrin, IP_mbcs.c_str(), port);
	m_session->set_sock_addr_in(&sockaddrin);
	return set();
}

bool Connector::send(const int len, const void* ptr)
{
	if( false == is_connected())
		return false;

	return m_peer->send(len, ptr);
}

bool Connector::is_connected()
{
	return m_session->is_connected();
}

int Connector::get_idx()
{
	return m_session->get_idx();
}

void Connector::on_connected(Session* session, RESULT_CODE::Type code)
{
    session->initialize();
    session->set_connected(true);

    if(SOCKET_ERROR == setsockopt(session->get_socket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
	{
		LOG_DEBUG(L"setsockopt SO_UPDATE_CONNECT_CONTEXT fail: %d [%d] [%d]", WSAGetLastError(), get_idx(), is_connected());
	}

	if( m_connect_callback )
	{
		m_connect_callback(code);
	}
}

void Connector::connect_failed(Session* session, RESULT_CODE::Type code)
{
	if( m_connect_callback )
	{
		m_connect_callback(code);
	}

    if (code != RESULT_CODE::SUCCESS) 
    {
        session->close_socket();
    }

    LOG_DEBUG(L"[%d] [%d] [%S:%d] [%d]", code, session->get_idx(), session->get_ip().c_str(), session->get_port(), get_idx());
    session->get_connect_context()->set_register(false);
}

void Connector::on_io_failed(Session* session, RESULT_CODE::Type code)
{
    LOG_DEBUG(L"[%d] [%d] [%S:%d] [%d]", WSAGetLastError(), session->get_idx(), session->get_ip().c_str(), session->get_port(), get_idx());

    session->disconnect(code);
}

void Connector::stop()
{
	if( is_connected())
		disconnect(RESULT_CODE::STOP_PROCESS);
}

} // namespace net

} // namespace light
