#include "stdafx.h"
#include "chat_server.h"

#include "Listener.h"
#include "GuiPrompt.h"

#include "CommandLineArguments.h"

#include <boost/bind.hpp>

chat_server::chat_server(const std::wstring& program_name, const std::wstring& arg) 
	: EnvironmentManager(program_name, arg)
{
}

chat_server::~chat_server()
{
}

bool chat_server::initialize()
{
	if( false == m_Config.Load() )
	{
		return false;
	}
	
	light::net::Listener::create_thread(m_Config.thread_count());

	m_Listener.reset(new light::net::Listener(boost::bind(&chat_server::peer_allocator, this, _1)));

    INT32 peer_count = 100;
    light::CommandLineArguments::has_if_assign(L"PEER_COUNT", [&](const std::wstring& value) {peer_count = _wtoi(value.c_str()); });

	if( FALSE == m_Listener->start(m_Config.Port(), peer_count))
	{
		stop();
		return false;
	}

	m_gui_prompt.reset(new light::GuiPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
    m_gui_prompt->start();
	
    on_complete();
	return true;
}

void chat_server::on_update()
{
	Sleep(100);

	m_Listener->on_update();
}

void chat_server::release()
{
	light::net::Listener::delete_thread();
}
