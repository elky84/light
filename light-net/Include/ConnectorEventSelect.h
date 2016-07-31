#pragma once

#include "EventSelectInterface.h"
#include "IoInterface.h"
#include "ConnectorInterface.h"
#include "TCPSocket.h"

#include <atomic>

namespace light
{

class Tick;

} //namespace light

namespace light
{

namespace net
{

class Peer;

class ConnectorEventSelect 
	: public EventSelectInterface
	, public ConnectorInterface
	, public IoInterface
{
protected:
	virtual void on_disconnect();	

	virtual void on_connect();

	virtual bool on_send();

	virtual bool on_receive();

	virtual void reuse();

	virtual bool on_receive(light::net::PACKET_SIZE len);

	virtual bool on_packet(light::net::PACKET_SIZE len, void* data);
public:
	ConnectorEventSelect(Peer* peer);

	virtual ~ConnectorEventSelect();

	virtual bool send(light::net::PACKET_SIZE len, const void* ptr);

	virtual bool disconnect(RESULT_CODE::Type code);

	virtual bool is_connected()
	{
		return m_connected;
	}

	void alive_check();

	bool connect(const char* ip, PORT port);

private:
    std::unique_ptr<LinearNormalBuffer> m_receive_buffer;

	std::unique_ptr<LinearNormalBuffer> m_send_buffer;

    std::atomic<bool> m_connected;

    std::unique_ptr<Tick> m_tick;

    TCPSocket m_tcp_socket;
};

} // namespace net

} // namespace light