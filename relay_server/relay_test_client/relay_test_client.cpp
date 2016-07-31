#include "stdafx.h"
#include "relay_test_client.h"

#include "server_peer.h"
#include "Connector.h"
#include "GuiPrompt.h"
#include "CommandLineArguments.h"

#include <boost/bind.hpp>

relay_test_client::relay_test_client(const std::wstring& program_name, const std::wstring& arg)
    : light::EnvironmentManager(program_name, arg)
{
}

relay_test_client::~relay_test_client()
{

}

bool relay_test_client::initialize()
{
    if (false == m_config.Load())
    {
        return false;
    }

    INT32 peer_count = 500;
    light::CommandLineArguments::has_if_assign(L"PEER_COUNT", [&](const std::wstring& value) {peer_count = _wtoi(value.c_str()); });

    ACTION_TYPE action = ACTION_TYPE::CHANNEL_BROADCAST;
    light::CommandLineArguments::has_if_assign(L"ACTION", [&](const std::wstring& value) {action = (ACTION_TYPE)_wtoi(value.c_str()); });

    light::net::Connector::create_thread(m_config.thread_count());
    for (int i = 0; i < peer_count; i++)
    {
        std::shared_ptr<light::net::Connector> connector(new light::net::Connector(new server_peer(i, action)));
        connector->set(m_config.IP(), m_config.Port());
        m_connectors.insert({ i, connector });
    }

    m_gui_prompt.reset(new light::GuiPrompt(boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();

    on_complete();
    return true;
}

void relay_test_client::on_update() 
{
    Sleep(1);
    for (MAP_CONNECTOR::iterator it = m_connectors.begin(); it != m_connectors.end(); ++it)
    {
        std::shared_ptr<light::net::Connector> connector = it->second;
        connector->on_update();
    }
}

void relay_test_client::release()
{
    light::net::Connector::delete_thread();
    m_connectors.clear();
}
