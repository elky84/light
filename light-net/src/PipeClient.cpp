#include "light-net.h"
#include "PipeClient.h"

namespace light
{

namespace net
{

PipeClient::PipeClient(const std::wstring& name, Peer* peer) 
: Pipe(name, peer)
{
}


PipeClient::~PipeClient()
{
}

bool PipeClient::connect()
{
	if(is_connected())
		return false;

	HANDLE pipe = CreateFile( get_name().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, GetPipeContextAccept()); 
	if(pipe == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(L"CreateFile Failed [%u]", GetLastError());
		return false;
	}

	set_pipe(pipe);
	switch (GetLastError()) 
	{ 
	case ERROR_IO_PENDING: // The overlapped connection in progress. 
		{
			LOG_DEBUG(L"try");
			try_connect();
			set();
		}
		break; 
	case ERROR_SUCCESS: // Client is already connected, so signal an event. 
		{
			set();
			SetEvent(GetPipeContextAccept()->get_event());
            LOG_DEBUG(L"connected");
		}
		break;
	default: // If an error occurs during the connect operation... 
		{
            LOG_DEBUG(L"failed. %d.", GetLastError());
			disconnect(RESULT_CODE::NOT_DEFINED_ACTION);
			return false;
		}
	} 

	return true;
}

bool PipeClient::disconnect(RESULT_CODE::Type code)
{
	LOG_INFO(L"[%p] [%d]", this, code);
	on_disconnect();
    return true;
}

void PipeClient::on_update()
{
	if(false == is_connected())
	{
		connect();
	}
	else
	{
		__super::on_update();
	}
}

bool PipeClient::release()
{
	if(false == ::DisconnectNamedPipe(get_pipe()))
	{
		LOG_INFO(L"disconnectNamedPipe failed [%d]", GetLastError());
	}

	__super::on_disconnect();
	return true;
}

void PipeClient::on_disconnect()
{
	release();
}

} // namespace net

} // namespace light
