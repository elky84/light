#pragma once

#include <map>
#include <vector>
#include <list>

#include <boost/function.hpp>

#include "IocpInterface.h"
#include "ListenInterface.h"
#include "PeerEx.h"
#include "spin_lock.h"
#include "NetStat.h"

namespace light
{

namespace net
{

class Session;

class Acceptor;

class Listener 
    : public IocpInterface
    , public ListenInterface
{
private:
	static const int DEFAULT_WAITING_QUEUE = 5;

	typedef std::map<int, Session*> MAP_SESSION;

	typedef boost::function<PeerEx* (const int& idx)> PeerAllocateFunc;

public:
	Listener(PeerAllocateFunc peer_alloc_function);

	virtual ~Listener();

	static void create_thread(int thread_count);

	static void delete_thread();

	bool start(PORT port, int waiting_queue_count = DEFAULT_WAITING_QUEUE);

	virtual bool disconnect(Session* session, RESULT_CODE::Type code);

	virtual bool send(Session* session, light::net::PACKET_SIZE len, void* ptr);
	
	bool send(int idx, light::net::PACKET_SIZE len, void* ptr);

	virtual void on_update();

	void stop(); 

    SOCKET get_socket()
    {
        return m_tcp_socket.get_socket();
    }

protected:
	virtual void on_connected(Session* session, RESULT_CODE::Type code);

	virtual void on_disconnect(Session* session, RESULT_CODE::Type code);

	virtual void on_io_failed(Session* session, RESULT_CODE::Type code);

	virtual void on_reuse(Session* session, RESULT_CODE::Type code);

	virtual void connect_failed(Session* session, RESULT_CODE::Type code);

	bool accept(Session* session);

protected:
	MAP_SESSION m_sessions;

private:
    PeerAllocateFunc m_peer_alloc_function;

    std::unique_ptr<Acceptor> m_acceptor;

    std::map< STAT, std::shared_ptr< Statistics<int> > > m_stats;

private:
    static std::vector<HANDLE> m_iocps;

    static HANDLE m_iocp;
};

} // namespace net

} // namespace light
