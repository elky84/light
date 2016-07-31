#pragma once

#include "EnvironmentManager.h"

#include "client_peer.h"
#include "Listener.h"

#include "NetworkConfig.h"

namespace light
{

class GuiPrompt;

} // namespace light


class chat_server 
	: public light::EnvironmentManager
{
public:
	chat_server(const std::wstring& program_name, const std::wstring& arg);
	~chat_server();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	inline light::net::PeerEx* peer_allocator(int idx)
	{
		client_peer* pPeer = new client_peer(idx);
		return pPeer;
	}

private:
	std::unique_ptr<light::net::Listener> m_Listener;

	std::unique_ptr<light::GuiPrompt> m_gui_prompt;

	light::net::NetworkConfig m_Config;
};
