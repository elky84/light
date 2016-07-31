#pragma once

#include <MSWSock.h>
#include <vector>
#include <memory>

#include "IocpInterface.h"
#include "ConnectorInterface.h"
#include "weak_raw_ptr.hpp"

#include "NetStat.h"

namespace light
{

namespace net
{

class Session;
class Peer;
class PeerManager;
class SessionManager;

class Connector 
	: public IocpInterface
	, public ConnectorInterface
{	
public:
	static void create_thread(int thread_count);

	static void delete_thread();

public:
	Connector(Peer* peer = NULL);

	virtual ~Connector();

	virtual bool set(const std::wstring& ip, PORT port);

	virtual bool connect();
	
	void disconnect(RESULT_CODE::Type code);

	bool is_connected();

	bool close_socket();

	int get_idx();

	virtual void on_update();

	bool send(const int len, const void* data);

	void stop();

	void on_connect_callback(boost::function<void (RESULT_CODE::Type)> callback)
	{
		m_connect_callback = callback;
	}

	void on_disconnect_callback(boost::function<void (RESULT_CODE::Type)> callback)
	{
		m_disconnect_callback = callback;
	}

protected:
	virtual void on_disconnect(Session* session, RESULT_CODE::Type code);

	virtual void on_io_failed(Session* session, RESULT_CODE::Type code);

	virtual void on_reuse(Session* session, RESULT_CODE::Type code);

	virtual void connect_failed(Session* session, RESULT_CODE::Type code);

private:
	bool set();

	virtual void on_connected(Session* session, RESULT_CODE::Type code);

	virtual bool disconnect(Session* session, RESULT_CODE::Type code);

protected:
    light::weak_raw_ptr<Session> m_session;

private:
    light::weak_raw_ptr<Peer> m_peer;

	static std::vector<HANDLE> m_iocps;

	LPFN_CONNECTEX m_lpfn_connect_ex;

	boost::function<void (RESULT_CODE::Type)> m_connect_callback;

	boost::function<void (RESULT_CODE::Type)> m_disconnect_callback;

    std::map< STAT, std::shared_ptr< Statistics<int> > > m_stats;
};

} // namespace net

} // namespace light
