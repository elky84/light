#pragma once

#include "Dispatcher.hpp"
#include "weak_raw_ptr.hpp"

#include <atomic>

namespace light
{

namespace net
{

class IoInterface;

class PacketStream;

class Peer
{
public:
	Peer(IoInterface* io_interface = NULL);

	virtual ~Peer();

public:
	virtual bool send(light::net::PACKET_SIZE len, const void* ptr);

	void disconnect(RESULT_CODE::Type code);

	void reuse();

	std::string get_ip()
	{
		char addr4_str[20];
		inet_ntop(AF_INET, &m_sock_addr_in->sin_addr, addr4_str, sizeof(addr4_str));
		return addr4_str;
	}

	void set_io_interface(IoInterface* io_interface) 
	{
        m_io_interface.exchange(io_interface);
	}

	void set_sock_addr_in(SOCKET_ADDR_IN& sockaddrin)
	{
		m_sock_addr_in.reset(&sockaddrin);
	}

	SOCKET_ADDR_IN* get_sock_addr_in()
	{
		return m_sock_addr_in.get();
	}

public:
	virtual void on_connect() = 0;

	virtual void on_disconnect() = 0;

	virtual void on_update() = 0;

	virtual PACKET_SIZE verify(PACKET_SIZE len, const void* ptr);

	virtual void on_receive(PACKET_SIZE len, const void* ptr);

protected:
	weak_raw_ptr<PACKET_DISPATCHER> m_dispatcher;

protected:
    std::atomic<IoInterface*> m_io_interface;

    weak_raw_ptr<SOCKET_ADDR_IN> m_sock_addr_in;
};

} // namespace net

} // namespace light
