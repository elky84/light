#include "light-net.h"
#include "IocpThreadManager.h"

#include "IocpInterface.h"

#include "IocpWorker.h"

#include <algorithm>

namespace light
{

namespace net
{

IocpThreadManager::IocpThreadManager()
{
}

IocpThreadManager::~IocpThreadManager()
{
	release();
}

void IocpThreadManager::add(HANDLE handle)
{
	scope_lock<spin_lock> cs(m_lock);
    m_threads.insert({ handle, std::shared_ptr<IocpWorker>(new IocpWorker(handle)) });
}

void IocpThreadManager::remove(HANDLE handle)
{
	scope_lock<spin_lock> cs(m_lock);

    auto it = m_threads.find(handle);
    if (it == m_threads.end())
        return;

    // before thread stop.
    it->second->stop();

    // erase. this code's mean call destructor  [=thread join].
    m_threads.erase(it);
}

void IocpThreadManager::release()
{
	scope_lock<spin_lock> cs(m_lock);
    std::for_each(m_threads.begin(), m_threads.end(), [](auto it) { it.second->stop(); });

	m_threads.clear();
}

} // namespace net

} // namespace light
