#pragma once

#include <memory>

namespace light
{

namespace net
{

class Session;
class IocpThreadManager;
class SessionManager;

class IocpInterface
{
public:
	IocpInterface();

	virtual ~IocpInterface();

    static void initialize();

    static void release();

	virtual void on_update() = 0;

	virtual bool disconnect(Session* session, RESULT_CODE::Type code) = 0;

	virtual void on_io_failed(Session* pcSession, RESULT_CODE::Type code) = 0;

	virtual void on_disconnect(Session* session, RESULT_CODE::Type code) = 0;

	virtual void on_connected(Session* session, RESULT_CODE::Type code) = 0;

	virtual void on_reuse(Session* session, RESULT_CODE::Type code) = 0;

	virtual void connect_failed(Session* session, RESULT_CODE::Type code) = 0;

protected:
	static HANDLE create_iocp_handle();

protected:
    static std::unique_ptr<SessionManager> m_session_manager;

    static std::unique_ptr<IocpThreadManager> m_iocp_thread_manager;
};

} // namespace net

} // namespace light
