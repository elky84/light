#include "light-net.h"
#include "EventSelectInterface.h"
#include "spin_lock.h"

namespace light
{

namespace net
{

EventSelectInterface::EventSelectInterface(const SOCKET& socket)
: m_socket_delegate(socket)
, m_event(WSACreateEvent())
{

}

EventSelectInterface::~EventSelectInterface()
{
	if(smart_instance()->remove(this))
	{
		while(!m_buffers.empty())
		{
			LinearNormalBuffer* buffer = m_buffers.front();
			m_buffers.pop();
			delete buffer;
		}
		SAFE_CLOSE_HANDLE(m_event);
	}
}

void EventSelectInterface::Proc()
{
	WSANETWORKEVENTS wsaEvent;
	WSAEnumNetworkEvents(m_socket_delegate, m_event, &wsaEvent);
	lock();
	if(wsaEvent.lNetworkEvents & FD_READ)
	{
		on_receive();
	}
	else if(wsaEvent.lNetworkEvents & FD_WRITE)
	{
		on_send();
	}
	else if(wsaEvent.lNetworkEvents & FD_CLOSE)
	{
		on_disconnect();
	}
	unlock();
}

bool EventSelectInterface::set()
{
	int nRet = WSAEventSelect(m_socket_delegate, m_event, FD_WRITE | FD_READ | FD_CLOSE);
	if(nRet == SOCKET_ERROR)
		return false;

	return smart_instance()->set(this);
}

bool EventSelectInterface::remove()
{
	if(smart_instance()->remove(this))
	{
		pop();
		SAFE_CLOSE_HANDLE(m_event);
		return true;
	}
	return false;
}

void EventSelectInterface::on_connect()
{
	smart_instance()->start();
	set();
}

void EventSelectInterface::on_disconnect()
{
	smart_instance()->stop();
	remove();
}

void EventSelectInterface::push(LinearNormalBuffer* buffer)
{
	lock();
	m_buffers.push(buffer);
	unlock();
}

void EventSelectInterface::pop()
{
	lock();
    while (!m_buffers.empty())
    {
        LinearNormalBuffer* buffer = m_buffers.front();
        m_buffers.pop();
        unlock(); // 버퍼 컨테이너를 빠져나왔으니.. unlock()

        on_packet(buffer->get_length(), buffer->get_head_buffer());
        delete buffer;

        lock(); // 로직 처리하고 다시 잠금.
    }
	unlock();
}

} // namespace net

} // namespace light