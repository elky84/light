#pragma once

#include "EnvironmentManager.h"

#include "NetworkConfig.h"
#include "weak_raw_ptr.hpp"

class server_peer;

namespace light
{

class GuiPrompt;

} // namespace light


namespace light
{

namespace net
{

class Connector;

} // namespace net

} // namespace light

class chat_client : public light::EnvironmentManager
{
public:
	chat_client(const std::wstring& program_name, const std::wstring& arg);
	~chat_client();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

	bool OnCommand(const std::wstring& str);

private:
	std::unique_ptr<light::GuiPrompt> m_gui_prompt;

	std::unique_ptr<light::net::Connector> m_connector;

    light::weak_raw_ptr<server_peer> m_test_peer;

	light::net::NetworkConfig m_Config;
};
