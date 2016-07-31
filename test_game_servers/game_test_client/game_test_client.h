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

class game_test_client : public light::EnvironmentManager
{
    typedef std::map<int, std::shared_ptr<light::net::Connector> > MAP_CONNECTOR;
    MAP_CONNECTOR m_connectors;

public:
    game_test_client(const std::wstring& program_name, const std::wstring& arg);
    ~game_test_client();

    virtual bool initialize();
    virtual void on_update();
    virtual void release();

private:
    light::net::NetworkConfig m_config;

    std::unique_ptr<light::GuiPrompt> m_gui_prompt;
};
