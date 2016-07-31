#pragma once

#include "EventWorker.h"
#include "LinearBuffer.h"
#include <queue>

#include "smart_singleton.h"

namespace light
{

class spin_lock;

} //namespace light

namespace light
{

namespace net
{

class EventSelectInterface 
	: public smart_singleton< EventWorker<EventSelectInterface> >
{
public:
	void pop();

	inline void lock()
	{
		m_lock.lock();
	}

	inline void unlock()
	{
		m_lock.unlock();
	}

	HANDLE get_event()
	{
		return m_event;
	}

	void Proc();

protected:
	EventSelectInterface(const SOCKET& socket);

	virtual ~EventSelectInterface();

	virtual bool on_send() = 0;

	virtual bool on_receive() = 0;

	virtual void on_disconnect();

	virtual void on_connect();

	virtual bool on_packet(light::net::PACKET_SIZE len, void* data) = 0;

	bool set();

	bool remove();

	void push(LinearNormalBuffer* buffer);

private:
	const SOCKET& m_socket_delegate;

	HANDLE m_event;

	std::queue<LinearNormalBuffer*> m_buffers;

protected:
	spin_lock m_lock;

};

} // namespace net

} // namespace light
