#pragma once

#include "EnvironmentManager.h"

#include "NetworkConfig.h"

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

class test_client : public light::EnvironmentManager
{
public:
	test_client(const std::wstring& program_name, const std::wstring& arg);
	~test_client();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	std::unique_ptr<light::GuiPrompt> m_gui_prompt;

	std::unique_ptr<light::net::Connector> m_connector;

	light::net::NetworkConfig m_config;
};
