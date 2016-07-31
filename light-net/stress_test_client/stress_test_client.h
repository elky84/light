#pragma once

#include <map>

#include "EnvironmentManager.h"
#include "NetworkConfig.h"

namespace light
{

class GuiPrompt;

namespace net
{

class Connector;

} // namespace net

} // namespace light

class stress_test_client : public light::EnvironmentManager
{
public:
	stress_test_client(const std::wstring& program_name, const std::wstring& arg);
	~stress_test_client();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
    light::net::NetworkConfig m_config;

    std::unique_ptr<light::GuiPrompt> m_gui_prompt;

    std::map<int, std::shared_ptr<light::net::Connector> > m_connectors;

    int sleep_time = 0;
};
