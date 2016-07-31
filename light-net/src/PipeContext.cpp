#include "light-net.h"
#include "PipeContext.h"
#include "Pipe.h"

namespace light
{

namespace net
{

PipeContext::PipeContext(Pipe* pipe, IO::Type io_type) 
: m_pipe(pipe)
, m_io_type(io_type)
{
	this->hEvent = CreateEvent(FALSE, FALSE, NULL, NULL);
}

PipeContext::~PipeContext()
{
	SAFE_CLOSE_HANDLE(this->hEvent);
}

void PipeContext::Proc()
{
	DWORD transfer;
	BOOL ret = GetOverlappedResult(
        m_pipe->get_pipe(), // handle to pipe 
		this, // OVERLAPPED structure 
		&transfer,            // bytes transferred 
		FALSE);            // do not wait 

	if(ret == FALSE)
	{
        m_pipe->on_disconnect();
		return;
	}

	if(ret == TRUE && transfer == 0)
	{
		if(io_type() == IO::CONNECT)
		{
            m_pipe->on_connect();
			return;
		}
		else
		{
            m_pipe->on_disconnect();
			return;

		}	
	}

	switch(io_type())
	{
	case IO::RECV:
		{
            m_pipe->on_receive(transfer);
		}
		break;
	case IO::SEND:
		{
            m_pipe->on_send(transfer);
		}
		break;
	default: 
		{
			LOG_ERROR(L"Invalid pipe state. %d", io_type()); 
            m_pipe->disconnect(RESULT_CODE::NOT_DEFINED_ACTION);
			return;
		}
	}
}

} // namespace net

} // namespace light
