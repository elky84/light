#include "light-net.h"
#include "Pipe.h"
#include "Peer.h"
#include "PipeContext.h"

namespace light
{

namespace net
{

EventWorker<PipeContext> Pipe::m_event_worker;

Pipe::Pipe(const std::wstring& name, Peer* peer)
: IoInterface(peer)
, m_name(L"\\\\.\\pipe\\" + name)
, m_pipe(INVALID_HANDLE_VALUE)
, m_try_connect(false)
, m_connected(false)
, m_sending(false)
, m_ContextAccept(this, IO::CONNECT)
, m_ContextRecv(this, IO::RECV)
, m_ContextSend(this, IO::SEND)
, m_send_buffer(new LinearNormalBuffer(PIPE_BUF_SIZE))
, m_receive_buffer(new LinearNormalBuffer(PIPE_BUF_SIZE))
{
	
}

Pipe::~Pipe()
{
}

bool Pipe::receive()
{
	if( false == is_connected())
		return false;

	DWORD nRecv = 0;
	if( FALSE == ReadFile(m_pipe, m_receive_buffer->get_head_buffer(), m_receive_buffer->get_remain_length(), &nRecv, GetPipeContextRecv()))
	{
		if(ERROR_IO_PENDING != GetLastError())
		{
			LOG_INFO(L"ReadFile Failed [%d] [%p, %d]", GetLastError(), m_receive_buffer->get_head_buffer(), m_receive_buffer->get_remain_length());
			return false;
		}
	}
	return true;
}

void Pipe::reuse()
{
}

bool Pipe::send(light::net::PACKET_SIZE len, const void* ptr)
{
	if(false == is_connected())
		return false;

	if( false == m_send_buffer->write(len, ptr))
	{
		LOG_INFO(L"m_send_buffer->write(%d, %p) Failed [%d]", len, ptr, GetLastError());
		return false;
	}

	scope_lock<spin_lock> cs(m_lock);
	if(TRUE == m_sending)
		return true;

	send();
	return true;
}

bool Pipe::send()
{
	DWORD written = 0;
	if(FALSE == WriteFile(m_pipe, m_send_buffer->get_head_buffer(), m_send_buffer->get_length(), &written, GetPipeContextSend()))
	{
		if(ERROR_IO_PENDING != GetLastError())
		{
			LOG_INFO(L"WriteFile(%d, %d, %d) Failed [%d]", m_pipe, m_send_buffer->get_head_buffer(), m_send_buffer->get_length(), GetLastError());
			disconnect(RESULT_CODE::SEND_FAILED);
			return false;
		}
	}

	m_sending = true;
	return true;
}

void Pipe::set()
{
	m_event_worker.set(GetPipeContextAccept());
	m_event_worker.set(GetPipeContextRecv());
	m_event_worker.set(GetPipeContextSend());
}

void Pipe::remove()
{
	m_event_worker.remove(GetPipeContextAccept());
	m_event_worker.remove(GetPipeContextRecv());
	m_event_worker.remove(GetPipeContextSend());
}

bool Pipe::on_receive(light::net::PACKET_SIZE len)
{
	m_receive_buffer->jump_tail(len);
	while(0 < m_receive_buffer->get_length())
	{
		int length = m_peer->verify(m_receive_buffer->get_length(), m_receive_buffer->get_head_buffer());
		if(length == 0) //0이면 패킷이 아직 덜온것
		{
			break;
		}
		else if (length == INVALID_FILE_SIZE)
		{
			disconnect(RESULT_CODE::INVALID_DATA_ON_VERIFY);
			return false;
		}
		else //다른 크기면 유효한걸로 봄.
		{
			m_peer->on_receive(length, m_receive_buffer->get_head_buffer());
			m_receive_buffer->jump_head(length);
		}
	}
	m_receive_buffer->compress();
	receive();
	return true;
}

void Pipe::on_connect()
{
	m_connected = true;
	m_try_connect = false;

	m_peer->on_connect();
	receive();
}

void Pipe::on_disconnect()
{
	remove();
    m_connected = false;

	m_peer->on_disconnect();

	m_receive_buffer->initialize();
	m_send_buffer->initialize();

	m_connected = false;
}

void Pipe::on_send(light::net::PACKET_SIZE len)
{
	scope_lock<spin_lock> cs(m_lock);
	m_send_buffer->jump_head(len);
	m_send_buffer->compress();
	m_sending = false;

	if( 0 != m_send_buffer->get_length())
	{
		send();
	}

}

void Pipe::start()
{
	m_event_worker.start();
}

void Pipe::release()
{
	m_event_worker.stop();
}

void Pipe::on_update()
{
	m_peer->on_update();
}

} // namespace net

} // namespace light
