#include "light-net.h"
#include "PeerManager.h"

namespace light
{

namespace net
{

PeerManager::PeerManager()
{

}

PeerManager::~PeerManager()
{
	release();
}

bool PeerManager::add(const int idx, Peer* peer)
{
	scope_lock<spin_lock> cs(m_lock);
    std::pair<MAP_PEER::iterator, bool> ret = m_peers.insert({ idx, peer });
	if( false == ret.second )
	{
		LOG_ERROR(L"failed [%d]", idx);  
		return false;
	}
	LOG_DEBUG(L"success [%d]", idx);  
	return true;
}

int PeerManager::count()
{
    scope_lock<spin_lock> cs(m_lock);
    return m_peers.size();
}

bool PeerManager::remove(const int idx)
{
	scope_lock<spin_lock> cs(m_lock);
	MAP_PEER::iterator it = m_peers.find(idx);
	if(it == m_peers.end())
	{
		LOG_ERROR(L"failed [%d]", idx);  
		return false;
	}

	m_peers.erase(it);
	LOG_DEBUG(L"success [%d]", idx);
	return true;
}

void PeerManager::release()
{
	scope_lock<spin_lock> cs(m_lock);
	m_peers.clear();
}

Peer* PeerManager::get(const int idx)
{
	scope_lock<spin_lock> cs(m_lock);
	MAP_PEER::iterator it = m_peers.find(idx);
	if(it == m_peers.end())
	{
		return NULL;
	}
	return it->second;
}

void PeerManager::on_update()
{
	scope_lock<spin_lock> cs(m_lock);
    std::for_each(m_peers.begin(), m_peers.end(), [](auto it) { it.second->on_update(); });
}

void PeerManager::broadcast(int len, void* data)
{
	scope_lock<spin_lock> cs(m_lock);
    std::for_each(m_peers.begin(), m_peers.end(), [&](auto it) { it.second->send(len, data); });
}

} // namespace net

} // namespace light
