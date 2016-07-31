#include "light-net.h"

#include "Acceptor.h"
#include "Session.h"
#include "Listener.h"
#include "SessionManager.h"
#include "IocpContext.h"

#include <MSWSock.h>

#pragma comment(lib,"mswsock.lib") 

namespace light
{

namespace net
{

Acceptor::Acceptor(Listener* listener) 
: m_listener(listener)
{
}

Acceptor::~Acceptor()
{
	LOG_DEBUG(L"complete");
}

bool Acceptor::accept(Session* session)
{
	if(session->get_socket() == INVALID_SOCKET)
	{
		if(false == session->create_socket())
		{
			LOG_ERROR(L"socket create failed. [%d] [%d]", session->get_idx(), WSAGetLastError());
			return false;
		}
	}

    session->initialize();
	session->set_iocp_interface(m_listener.get());

    session->get_connect_context()->set_register(true);
	if( FALSE == AcceptEx(m_listener->get_socket(), session->get_socket(), session->get_connect_context()->get_head_buffer(), 0, ACCEPT_ADDRESS_LENGTH, ACCEPT_ADDRESS_LENGTH, NULL, session->get_connect_context()))
	{
        DWORD err = WSAGetLastError();
        if (err == ERROR_IO_PENDING)
        {
            LOG_TRACE(L"[%d] io_pending [%d]", session->get_idx(), err);
        }
		else if(err != WSAEWOULDBLOCK)
		{
			LOG_ERROR(L"[%d] error [%d]", session->get_idx(), err);
            session->get_connect_context()->set_register(false);
			return false;
		}
	}

    LOG_TRACE(L"[%d] success", session->get_idx());
	return true;
}

} // namespace net

} // namespace light