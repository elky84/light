#pragma once

#include "EnvironmentManager.h"

#include "Thread.h"
#include "Listener.h"


#include "P2PRelayPeerEx.h"

#include "NetworkConfig.h"

class relay_server : public light::EnvironmentManager , public light::Singleton<relay_server>
{
public:
	relay_server(const std::wstring& program_name, const std::wstring& arg);
	~relay_server();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	inline light::net::PeerEx* peer_allocator(int idx)
	{
		return new light::net::P2PRelayPeerEx(idx);
	}

	std::unique_ptr<light::net::Listener> m_Listener;

	light::net::NetworkConfig m_Config;
};
