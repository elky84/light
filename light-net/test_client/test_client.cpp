#include "stdafx.h"
#include "test_client.h"

#include "server_peer.h"
#include "GuiPrompt.h"
#include "Connector.h"

#include <boost/bind.hpp>

test_client::test_client(const std::wstring& program_name, const std::wstring& arg) 
	: EnvironmentManager(program_name, arg)
{
	
}

test_client::~test_client()
{
	
}

bool test_client::initialize()
{
	if( false == m_config.Load() )
	{
		return false;
	}

	light::net::Connector::create_thread(m_config.thread_count());

	m_connector.reset(new light::net::Connector(new server_peer()));
	m_connector->set(m_config.IP(), m_config.Port());

	m_gui_prompt.reset(new light::GuiPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();

	on_complete();

	return true;
}

void test_client::on_update()
{
	Sleep(100);

	m_connector->on_update();
}

void test_client::release()
{
	light::net::Connector::delete_thread();
    m_connector.reset();
}