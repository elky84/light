#pragma once

#include "EnvironmentManager.h"

#include "client_peer.h"
#include "Listener.h"

#include "NetworkConfig.h"

#include "json_parser.h"

#include <redisclient/redisasyncclient.h>

class channel_manager;

namespace light
{

class GuiPrompt;

} // namespace light


class game_server 
	: public light::EnvironmentManager
{
public:
	game_server(const std::wstring& program_name, const std::wstring& arg);
	~game_server();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

public:
    void pve_subscribe_handler(const std::vector<char> &buf);
    
    void pve_list_subscribe_handler(const std::vector<char> &buf);
    
    virtual void stop();

private:
    light::net::PeerEx* peer_allocator(int idx);

private:
	std::unique_ptr<light::net::Listener> m_listener;

	std::unique_ptr<light::GuiPrompt> m_gui_prompt;

	light::net::NetworkConfig m_config;

    std::unique_ptr<channel_manager> m_channel_manager;

    std::string m_host;

    unsigned short m_port = 0;

private:
    boost::asio::io_service m_io_service;

    light::json::parser m_parser;

    std::unique_ptr<RedisAsyncClient> m_pve_subscriber;

    std::unique_ptr<RedisAsyncClient> m_pve_list_subscriber;
};
