#include "stdafx.h"
#include "stress_test_client.h"

#include "server_peer.h"
#include "Connector.h"
#include "GuiPrompt.h"
#include "CommandLineArguments.h"

#include <boost/bind.hpp>

stress_test_client::stress_test_client(const std::wstring& program_name, const std::wstring& arg)
	: light::EnvironmentManager(program_name, arg)
{
}

stress_test_client::~stress_test_client()
{
	
}

bool stress_test_client::initialize()
{
    if (false == m_config.Load())
    {
        return false;
    }

    INT32 peer_count = 1;
    light::CommandLineArguments::has_if_assign(L"PEER_COUNT", [&](const std::wstring& value) {peer_count = _wtoi(value.c_str()); });

    ACTION_TYPE action = ACTION_TYPE::TRY_CONNECT_DISCONNECT;
    light::CommandLineArguments::has_if_assign(L"ACTION", [&](const std::wstring& value) {action = (ACTION_TYPE)_wtoi(value.c_str()); });

    light::CommandLineArguments::has_if_assign(L"SLEEP", [&](const std::wstring& value) {sleep_time = _wtoi(value.c_str()); });

    light::net::Connector::create_thread(m_config.thread_count());
	for(int i = 0; i < peer_count; i++)
	{
        std::shared_ptr<light::net::Connector> connector(new light::net::Connector(new server_peer(i, action)));
        connector->set(m_config.IP(), m_config.Port());
		m_connectors.insert_or_assign(i, connector);
	}

    m_gui_prompt.reset(new light::GuiPrompt(boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();

    on_complete();
	return true;
}

void stress_test_client::on_update()
{
	Sleep(sleep_time);
    std::for_each(m_connectors.begin(), m_connectors.end(), [](auto it) { it.second->on_update(); });
}

void stress_test_client::release()
{
    std::for_each(m_connectors.begin(), m_connectors.end(), [](auto it) { it.second->stop(); });
    light::net::Connector::delete_thread();
    m_connectors.clear();
}
