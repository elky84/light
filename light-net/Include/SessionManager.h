#pragma once

#include <map>
#include <queue>

#include "spin_lock.h"

namespace light
{

namespace net
{

class Peer;
class Session;

class SessionManager
{
	typedef std::map<int, Session*> MAP_SESSION;

	MAP_SESSION m_using_sessions;

	std::queue<Session* > m_not_using_sessions;

    spin_lock m_lock;

public:
	SessionManager();

	~SessionManager();

	Session* pop();

    int not_using_session_count();

    int using_session_count();

	void push(Session* session);

	Session* get(const int idx);

	void alloc(const int count);

	inline void lock()
    {
        m_lock.lock();
    }

	inline void unlock()
    {
        m_lock.unlock();
    }
};

} // namespace net

} // namespace light
