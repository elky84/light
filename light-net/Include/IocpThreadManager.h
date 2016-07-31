#pragma once

#include <map>
#include <memory>

#include "spin_lock.h"

namespace light
{

namespace net
{

class IocpWorker;

typedef std::map<HANDLE, std::shared_ptr<IocpWorker> > MAP_THREAD;

class IocpThreadManager
{
public:
	IocpThreadManager();

	virtual ~IocpThreadManager();

	void add(HANDLE handle);
	
	void remove(HANDLE handle);

	void release();

private:
	MAP_THREAD m_threads;

	spin_lock m_lock;
};

} // namespace net

} // namespace light
