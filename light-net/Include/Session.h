#pragma once

#include "IoInterface.h"
#include "TCPSocket.h"
#include "spin_lock.h"

#include <atomic>

#include "NetStat.h"

namespace light
{

namespace net
{

class IocpContext;
class PeerEx;
class IocpInterface;

class Session 
	: public IoInterface
{
public:
	Session(const int idx, IocpInterface* iocp_interface = NULL);

	virtual ~Session();

	void set_iocp_interface(IocpInterface* iocp_interface)
	{
		m_iocp_interface = iocp_interface;
	}
	
    void set_sock_addr_in(const SOCKET_ADDR_IN* sockaddr);

    SOCKET_ADDR_IN* get_sock_addr_in();

    std::string get_ip();

    int get_port();

    bool is_connected();
	
	virtual bool disconnect(RESULT_CODE::Type code);

	void initialize();

	virtual bool send(light::net::PACKET_SIZE len, const void* ptr);

    bool close_socket()
    {
        return m_tcp_socket.close_socket();
    }
	
    bool check_reuse();

	SOCKET get_socket()
	{
		return m_tcp_socket.get_socket();
	}
	
	const int get_idx() const
	{
		return m_idx;
	}

    bool create_socket()
    {
        return m_tcp_socket.create_socket();
    }

	void set_connected(bool flag);

	void reuse();

	void set_peer(Peer* peer);

	void clear_peer();

public:
	void connect_failed(RESULT_CODE::Type code);

	void disconnect_process();

    bool is_register() const;

public: // IocpWorker에서 불려지는 메소드
	void on_connect(RESULT_CODE::Type code);

	void on_disconnect(RESULT_CODE::Type code);

	void on_send(DWORD sended);

	void on_reuse(RESULT_CODE::Type code);

	virtual bool on_receive(light::net::PACKET_SIZE len);

public:
	IocpContext* get_connect_context()
	{
		return m_connect_context.get();
	}

    bool disconnect()
	{
        if (m_disconnecting.load() == true)
            return false;

        m_disconnecting.exchange(true);
        return true;
	}

    bool is_disconnecting() const
    {
        return m_disconnecting.load();
    }

private:
    virtual bool send();

    virtual bool receive();

protected:
	std::string m_ip;

    std::atomic<bool> m_connected;

	std::atomic<bool> m_disconnecting;

	const int m_idx;	

	IocpInterface* m_iocp_interface;

    std::unique_ptr<IocpContext> m_send_context;

    std::unique_ptr<IocpContext> m_receive_context;

    std::unique_ptr<IocpContext> m_connect_context;

    std::unique_ptr<IocpContext> m_reuse_context;

    spin_lock m_send_lock;

    TCPSocket m_tcp_socket;

    std::map< STAT, std::shared_ptr< Statistics<int> > > m_stats;
};

} // namespace net

} // namespace light
