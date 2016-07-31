#pragma once

#include "EnvironmentManager.h"


#include "client_peer.h"
#include "Listener.h"

#include "NetworkConfig.h"

namespace light
{

class GuiPrompt;

} // namespace light


class test_server 
	: public light::EnvironmentManager
{
public:
	test_server(const std::wstring& program_name, const std::wstring& arg);
	~test_server();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	inline light::net::PeerEx* peer_allocator(int idx)
	{
		client_peer* peer = new client_peer(idx);
		return peer;
	}

private:
	std::unique_ptr<light::net::Listener> m_listener;

	std::unique_ptr<light::GuiPrompt> m_gui_prompt;

	light::net::NetworkConfig m_config;
};
