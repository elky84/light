#include "light-net.h"
#include "IocpWorker.h"

#include "Session.h"
#include "IocpContext.h"

namespace light
{

namespace net
{

IocpWorker::IocpWorker(HANDLE iocp)
: m_iocp(iocp)
, m_thread(boost::bind(&IocpWorker::on_update, this))
{
    m_thread.start();
}

IocpWorker::~IocpWorker()
{
}

void IocpWorker::on_update()
{
	DWORD transfer = 0;
	Session* session = NULL;
	IocpContext* iocp_context = NULL;

    BOOL ret = GetQueuedCompletionStatus(m_iocp, &transfer, (LPDWORD)&session, (LPOVERLAPPED*)&iocp_context, INFINITE);
	if(iocp_context == NULL) //스레드 종료 이벤트 수신.
	{
		return;
	}

	switch(iocp_context->io_type() )
	{
	case IO::REUSE:
		{
            LOG_TRACE(L"[%d] IOType[%d] Ret[%d] Transfer[%u] iocp_context[%p]", WSAGetLastError(), iocp_context->io_type(), ret, transfer, iocp_context);
            if( TRUE == ret)
                session->on_reuse(RESULT_CODE::SUCCESS);
            else
                session->on_reuse(RESULT_CODE::REUSE_FAILED);
        }
		break;
	case IO::CONNECT:
		{
			LOG_TRACE(L"[%d] IOType[%d] Ret[%d] Transfer[%u] iocp_context[%p]", WSAGetLastError(), iocp_context->io_type(), ret, transfer, iocp_context);
            session = iocp_context->get_session();
            iocp_context->end();
			if( TRUE == ret)
			{
                session->on_connect(RESULT_CODE::SUCCESS);
			}
			else
			{
                session->connect_failed(RESULT_CODE::CONNECT_FAILED_WORKER);
			}
		}
		break;
	case IO::SEND:
		{
            session->on_send(transfer);
    }
		break;
	case IO::RECV:
		{
            session->on_receive(transfer);
		}
		break;
	default:
		LOG_ERROR(L"[%d] IOType[%d] Ret[%d] Transfer[%u] iocp_context[%p]", WSAGetLastError(), iocp_context->io_type(), ret, transfer, iocp_context);
		break;
	}
}

void IocpWorker::stop()
{
    m_thread.stop();
    if (INVALID_HANDLE_VALUE != m_iocp)
    {
        PostQueuedCompletionStatus(m_iocp, 0, NULL, NULL);
    }

    m_thread.join();
}

} // namespace net

} // namespace light