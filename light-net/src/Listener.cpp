#include "light-net.h"
#include "Listener.h"

#include "SessionManager.h"
#include "Session.h"
#include "IocpThreadManager.h"
#include "Acceptor.h"
#include "IocpContext.h"
#include "Repository.h"

#include <MSWSock.h>

namespace light
{

namespace net
{

HANDLE Listener::m_iocp = INVALID_HANDLE_VALUE;
std::vector<HANDLE> Listener::m_iocps;

Listener::Listener(PeerAllocateFunc peer_alloc_function)
: m_peer_alloc_function(peer_alloc_function)
{
	m_acceptor.reset(new Acceptor(this));

    auto repository = Repository<STAT, Statistics<int> >::Instance();
    m_stats.insert({ STAT::ACCEPT, repository->get(STAT::ACCEPT) });
}

Listener::~Listener()
{
}

bool Listener::start(PORT port, int waiting_queue_count)
{
	if(m_iocp == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(L"Fail. m_iocp == INVALID_HANDLE_VALUE. you have to call create_thread Method.");
		return false;
	}

	if( false == bind(port))
	{
		LOG_ERROR(L"bind() Fail");
		return false;
	}

	if( false == listen(waiting_queue_count))
	{
		LOG_ERROR(L"listen() Fail");
		return false;
	}

	m_session_manager->alloc(waiting_queue_count);
	for(int i = 0; i < waiting_queue_count; i++)
	{
		Session* session = m_session_manager->pop();
		if(false == accept(session))
		{
			LOG_ERROR(L"accept() Fail. %d", session->get_idx());
			return false;
		}

		session->set_peer(m_peer_alloc_function(session->get_idx()));
        m_sessions.insert({ session->get_idx(), session });
	}

    HANDLE handle_iocp = CreateIoCompletionPort((HANDLE)m_tcp_socket.get_socket(), m_iocp, 0, 0);
    if (handle_iocp == NULL)
    {
        LOG_ERROR(L"CreateIoCompletionPort() handle_iocp Fail.");
        return false;
    }

	LOG_INFO(L"success [%S:%d] [%d]", get_local_ip_w().c_str(), port, waiting_queue_count);
	return true;
}

void Listener::on_connected(Session* session, RESULT_CODE::Type code)
{
    session->initialize();
    session->set_connected(true);

	if(setsockopt(session->get_socket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char* )&m_tcp_socket.get_socket(), sizeof(SOCKET)))
	{
		LOG_ERROR(L"setsockopt SO_UPDATE_ACCEPT_CONTEXT fail: %d [%d]", WSAGetLastError(), session->get_idx());
        session->set_connected(false);
        return;
	}

	int localLen, remoteLen;
	sockaddr_in* pLocal,* pRemote;
	GetAcceptExSockaddrs(session->get_connect_context()->get_head_buffer(), 0, ACCEPT_ADDRESS_LENGTH, ACCEPT_ADDRESS_LENGTH, (sockaddr** )&pLocal, &localLen, (sockaddr** )&pRemote, &remoteLen);
	session->set_sock_addr_in(pRemote);

	HANDLE hIocp = CreateIoCompletionPort((HANDLE)session->get_socket(), m_iocps[session->get_idx() % m_iocps.size()], (ULONG_PTR)session, 0); //억셉트된 소켓을 IOCP에 참여
	if(INVALID_HANDLE_VALUE == hIocp)
	{
		LOG_ERROR(L"CreateIoCompletionPort fail: %d [%d]", WSAGetLastError(), session->get_idx());
        session->set_connected(false);
        return;
	}

    LOG_INFO(L"success [%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());
}

bool Listener::disconnect(Session* session, RESULT_CODE::Type code)
{
    if (false == session->is_connected())
    {
        LOG_INFO(L"not connected session. [%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
            m_session_manager->not_using_session_count());
        return false;
    }

    if (false == session->disconnect())
    {
        LOG_INFO(L"already disconntecting. [%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
            m_session_manager->not_using_session_count());

        return false;
    }

    LOG_TRACE(L"success [%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());

    session->on_disconnect(code);
    return true;
}

void Listener::on_io_failed(Session* session, RESULT_CODE::Type code)
{
    LOG_INFO(L"[%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());

    session->disconnect(code);
}

void Listener::on_update()
{
}
	
void Listener::on_disconnect(Session* session, RESULT_CODE::Type code)
{
    session->disconnect_process();

    LOG_INFO(L"success [%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());
}

void Listener::on_reuse(Session* session, RESULT_CODE::Type code)
{
    if (m_acceptor.get() == NULL)
        return;

    LOG_TRACE(L"[%d] [%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), code, m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());

    accept(session);

    while (Session* not_using_session = m_session_manager->pop())
    {
        LOG_INFO(L"reuse not using sessions [%d] [%d] using_sessions[%d] not_using_sessions[%d]", not_using_session->get_idx(), code,
            m_session_manager->using_session_count(), m_session_manager->not_using_session_count());
        
        accept(not_using_session);
        Sleep(1); // anti infinite-loop.
    }
}

void Listener::connect_failed(Session* session, RESULT_CODE::Type code)
{
    LOG_INFO(L"[%d] using_sessions[%d] not_using_sessions[%d]", session->get_idx(), m_session_manager->using_session_count(), 
        m_session_manager->not_using_session_count());

    session->on_reuse(code);
}

void Listener::create_thread(int thread_count)
{
    IocpInterface::initialize();
    
	if(m_iocp == INVALID_HANDLE_VALUE)
	{
		m_iocp = create_iocp_handle();
        m_iocp_thread_manager->add(m_iocp);
	}

    for (int n = 0; n < thread_count; ++n)
    {
        HANDLE iocp = create_iocp_handle();
        m_iocp_thread_manager->add(iocp);
        m_iocps.push_back(iocp);
    }
}

void Listener::delete_thread()
{
    std::for_each(m_iocps.begin(), m_iocps.end(), [&](auto iocp) {m_iocp_thread_manager->remove(iocp); });
	m_iocp_thread_manager->remove(m_iocp);
    IocpInterface::release();
}

bool Listener::send(int idx, light::net::PACKET_SIZE len, void* ptr)
{
    auto session = m_session_manager->get(idx);
	if(session == NULL || session->get_peer() == NULL)
		return false;

    return session->get_peer()->send(len, ptr);
}

bool Listener::send(Session* session, light::net::PACKET_SIZE len, void* ptr)
{
	return session->send(len, ptr);
}

bool Listener::accept(Session* session)
{
    if (m_acceptor.get() == NULL)
        return false;

    m_stats[STAT::ACCEPT]->record(1);
	if( false == m_acceptor->accept(session))
	{
        m_session_manager->push(session);
        return false;
	}

	return true;
}

void Listener::stop()
{
	if(m_acceptor.get() == NULL)
		return;

    m_tcp_socket.close_socket();
    m_acceptor.reset();

	for(MAP_SESSION::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it) //모든 세션을 클리어 해준다.
	{
		Session* session = it->second;
        if(session->is_connected())
		    session->disconnect(RESULT_CODE::STOP_PROCESS);
	}
    m_sessions.clear();
}

} // namespace net

} // namespace light
