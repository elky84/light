#include "light-net.h"
#include "IocpInterface.h"

#include "IocpThreadManager.h"
#include "SessionManager.h"
#include "NetStat.h"

namespace light
{

namespace net
{

std::unique_ptr<SessionManager> IocpInterface::m_session_manager;
std::unique_ptr<IocpThreadManager> IocpInterface::m_iocp_thread_manager;

IocpInterface::IocpInterface()
{
}

IocpInterface::~IocpInterface()
{
}

void IocpInterface::initialize()
{
    NetStat::initialize();
    m_session_manager.reset(new SessionManager());
    m_iocp_thread_manager.reset(new IocpThreadManager());
}

void IocpInterface::release()
{
    m_iocp_thread_manager.reset();
    m_session_manager.reset();
    NetStat::release();
}

HANDLE IocpInterface::create_iocp_handle()
{
	HANDLE iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(iocp_handle == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(L"CreateIoCompletionPort() iocp_handle Fail.");
	}
	return iocp_handle;
}

} // namespace net

} // namespace light
