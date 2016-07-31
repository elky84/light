#include "light-net.h"
#include "SessionManager.h"
#include "Session.h"
#include "Peer.h"

namespace light
{

namespace net
{

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    scope_lock<spin_lock> cs(m_lock);
    for (auto it = m_using_sessions.begin(); it != m_using_sessions.end();)
	{
		Session* session = it->second;
        m_not_using_sessions.push(session);
        it = m_using_sessions.erase(it);
	}

    while (!m_not_using_sessions.empty())
	{
        Session* session = m_not_using_sessions.front();
        session->clear_peer();
		SAFE_DELETE(session);

        m_not_using_sessions.pop();
    }
}

void SessionManager::alloc(const int count)
{
	for(int i = 0; i < count; i++)
	{
		m_not_using_sessions.push(new Session(m_using_sessions.size() + m_not_using_sessions.size()));
	}
}

int SessionManager::not_using_session_count()
{
    scope_lock<spin_lock> cs(m_lock);
    return m_not_using_sessions.size();
}


int SessionManager::using_session_count()
{
    scope_lock<spin_lock> cs(m_lock);
    return m_using_sessions.size();
}


Session* SessionManager::pop()
{
	scope_lock<spin_lock> cs(m_lock);
	Session* session = NULL;
	if( true == m_not_using_sessions.empty()) 
	{
		return NULL;
	}
	else
	{
        session = m_not_using_sessions.front();
		m_not_using_sessions.pop();
	}

    m_using_sessions.insert({ session->get_idx(), session });
    LOG_TRACE(L"[%d] returned using_sessions. [%u] [%u]", session->get_idx(), m_using_sessions.size(), m_not_using_sessions.size());
    return session;
}

void SessionManager::push(Session* session)
{
	scope_lock<spin_lock> cs(m_lock);
    if (session == NULL)
    {
        LOG_ERROR(L"session is NULL. [%u] [%u]", m_using_sessions.size(), m_not_using_sessions.size());
        return;
    }

    session->close_socket();

    m_not_using_sessions.push(session); //무조건 큐에 넣어 재활용하자.

    auto it = m_using_sessions.find(session->get_idx());
    if (it == m_using_sessions.end())
    {
        LOG_ERROR(L"[%d] not find in using_sessions. [%u] [%u]", session->get_idx(), m_using_sessions.size(), m_not_using_sessions.size());
        return;
    }

	m_using_sessions.erase(it); 
    LOG_TRACE(L"[%d] push using_sessions. [%u] [%u]", session->get_idx(), m_using_sessions.size(), m_not_using_sessions.size());
}

Session* SessionManager::get(const int idx)
{
	scope_lock<spin_lock> cs(m_lock);
	MAP_SESSION::iterator it = m_using_sessions.find(idx);
	if(it == m_using_sessions.end())
	{
		LOG_ERROR(L"not find session [%d]", idx);
		return NULL;
	}

	return it->second;
}

} // namespace net

} // namespace light
