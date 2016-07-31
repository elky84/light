#pragma once

#include "PipeContext.h"
#include "IoInterface.h"
#include "LinearBuffer.h"
#include "EventWorker.h"

namespace light
{

namespace net
{

class PipeContext;
class Peer;

const int PIPE_BUF_SIZE = 1024 * 8;

class Pipe : public IoInterface
{
	friend class PipeContext;
public:
	Pipe(const std::wstring& name, Peer* peer);

	virtual ~Pipe();

	virtual void on_update() = 0;

	void start();

	virtual bool send(light::net::PACKET_SIZE len, const void* ptr);

	const HANDLE& get_pipe()
	{
		return m_pipe;
	}

	char* get_receive_head_buffer()
	{
		return m_receive_buffer->get_head_buffer();
	}

	BOOL is_connected()
	{
		return m_connected;
	}

	static void release();

protected:

	BOOL is_try_connect()
	{
		return m_try_connect;
	}

	void try_connect()
	{
		m_try_connect = TRUE;
	}

	void set_pipe(HANDLE& pipe)
	{
		m_pipe = pipe;
	}

	const std::wstring& get_name()
	{
		return m_name;
	}

	PipeContext* GetPipeContextAccept()
	{
		return &m_ContextAccept;
	}

	PipeContext* GetPipeContextRecv()
	{
		return &m_ContextRecv;
	}

	PipeContext* GetPipeContextSend()
	{
		return &m_ContextSend;
	}

	void set();

	void remove();

	bool receive();

	virtual void reuse();

protected:
	virtual void on_connect();

	virtual bool on_receive(light::net::PACKET_SIZE len);

	virtual void on_send(light::net::PACKET_SIZE len);

	virtual void on_disconnect();

private:
	bool send();

private:
    bool m_connected;

    bool m_try_connect;

	bool m_sending;

	HANDLE m_pipe;

	std::wstring m_name;

	PipeContext m_ContextAccept;

	PipeContext m_ContextRecv;

	PipeContext m_ContextSend;

    std::unique_ptr<LinearNormalBuffer> m_send_buffer;

    std::unique_ptr<LinearNormalBuffer> m_receive_buffer;

	spin_lock m_lock;

	static EventWorker<PipeContext> m_event_worker;
};

} // namespace net

} // namespace light
