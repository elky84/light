#pragma once

#include "Thread.h"

namespace light
{

namespace net
{

class IocpContext;
class Session;

class IocpWorker 
{
public:
	IocpWorker(HANDLE iocp);

	~IocpWorker();

	void on_update();

	void stop();

    bool is_active()
    {
        return m_thread.is_active();
    }

private:
	HANDLE m_iocp;

    Thread m_thread;
};

} // namespace net

} // namespace light
