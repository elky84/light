#include "stdafx.h"
#include "test_server.h"

#include "Listener.h"
#include "GuiPrompt.h"
#include "CommandLineArguments.h"

#include <boost/bind.hpp>

test_server::test_server(const std::wstring& program_name, const std::wstring& arg)
	: EnvironmentManager(program_name, arg)
{
}

test_server::~test_server()
{
}

bool test_server::initialize()
{
	if( false == m_config.Load() )
	{
		return false;
	}
	
	light::net::Listener::create_thread(m_config.thread_count());

	m_listener.reset(new light::net::Listener(boost::bind(&test_server::peer_allocator, this, _1)));

    INT32 peer_count = 100;
    light::CommandLineArguments::has_if_assign(L"PEER_COUNT", [&](const std::wstring& value) {peer_count = _wtoi(value.c_str()); });

	if( FALSE == m_listener->start(m_config.Port(), peer_count))
	{
		stop();
		return false;
	}

	m_gui_prompt.reset(new light::GuiPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();

	on_complete();
	return true;
}

void test_server::on_update()
{
	Sleep(100);
	m_listener->on_update();
}

void test_server::release()
{
    m_listener->stop();
    light::net::Listener::delete_thread();
    m_listener.reset();
}
