#include "light-net.h"
#include "PipeServer.h"

namespace light
{

namespace net
{

PipeServer::PipeServer(const std::wstring& name, Peer* peer) 
: Pipe(name, peer)
{
}

PipeServer::~PipeServer()
{
}

bool PipeServer::listen()
{
	if(is_connected())
		return false;

	HANDLE pipe = CreateNamedPipe( get_name().c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 
		PIPE_BUF_SIZE, PIPE_BUF_SIZE, NMPWAIT_USE_DEFAULT_WAIT, NULL);
	if(pipe == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(L"CreateNamedPipe Failed [%d]", GetLastError());
		return false;
	}

	set_pipe(pipe);
	ConnectNamedPipe(get_pipe(), GetPipeContextAccept());
	
	switch (GetLastError()) 
	{ 
	case ERROR_IO_PENDING: // The overlapped connection in progress. 
		{
			LOG_DEBUG(L"ConnectNamedPipe Try [%d]");
			try_connect();
			set();
		}
		break; 
	case ERROR_PIPE_CONNECTED: // Client is already connected, so signal an event. 
		{
			set();
			SetEvent(GetPipeContextAccept()->get_event());

			LOG_INFO(L"ConnectNamedPipe Connected [%d]");
			on_connect();
		}
		break;
	default: // If an error occurs during the connect operation... 
		{
			LOG_INFO(L"ConnectNamedPipe failed. %d.", GetLastError());
			CloseHandle(get_pipe());
			return false;
		}
	} 
	return true;
}

bool PipeServer::disconnect(RESULT_CODE::Type code)
{
	LOG_INFO(L"%s [%p] [%d]", __FUNCTIONW__, this, code);
	release();
	on_disconnect();
    return true;
}

void PipeServer::on_update()
{
	if(TRUE == is_connected())
	{
		__super::on_update();
	}
}

void PipeServer::on_disconnect()
{
	__super::on_disconnect();
	listen();
}

bool PipeServer::release()
{
	if(false == DisconnectNamedPipe(get_pipe()))
	{
		LOG_INFO(L"DisconnectNamedPipe failed [%d]", GetLastError());
		return false;
	}

	__super::on_disconnect();
	return true;
}

} // namespace net

} // namespace light
