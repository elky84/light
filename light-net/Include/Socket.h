#pragma once

#include <stdio.h>
#include <stdarg.h>

namespace light
{

namespace net
{

class Socket
{
    // 두개의 클래스를 제외하곤 상속 불가능하게 강제.
    friend class TCPSocket;
    friend class UDPSocket;

private:
    Socket();

public:
	virtual ~Socket();

	bool close_socket();

	static bool close_socket(SOCKET& socket);

	inline const SOCKET& get_socket()
    {
        return m_socket;
    }

	inline SOCKET_ADDR_IN* get_sock_addr_in()
	{
		return &m_sock_addr_in;
	}

    void set_sock_addr_in(const SOCKET_ADDR_IN* sockaddr)
    {
        m_sock_addr_in = *sockaddr;
    }

protected:
	virtual bool create_socket() = 0;

protected:
	SOCKET m_socket;

	SOCKET_ADDR_IN m_sock_addr_in;
};

} // namespace net

} // namespace light
