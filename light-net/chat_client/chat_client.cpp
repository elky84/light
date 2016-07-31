#include "stdafx.h"
#include "chat_client.h"

#include "server_peer.h"
#include "GuiPrompt.h"
#include "Connector.h"

#include <boost/bind.hpp>

chat_client::chat_client(const std::wstring& program_name, const std::wstring& arg) 
	: EnvironmentManager(program_name, arg)
{
	
}

chat_client::~chat_client()
{
	
}

bool chat_client::initialize()
{
	if( false == m_Config.Load() )
	{
		return false;
	}

	light::net::Connector::create_thread(m_Config.thread_count());

    m_test_peer.reset(new server_peer());
	m_connector.reset(new light::net::Connector(m_test_peer.get()));
	m_connector->set(m_Config.IP(), m_Config.Port());

	m_gui_prompt.reset(new light::GuiPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance()), get_program_name()));
	m_gui_prompt->callback( boost::bind(&chat_client::OnCommand, this, _1) );
    m_gui_prompt->start();

	on_complete();
	return true;
}

void chat_client::on_update()
{
	Sleep(100);

	m_connector->on_update();
}

void chat_client::release()
{
	light::net::Connector::delete_thread();
}

bool chat_client::OnCommand(const std::wstring& str)
{
	LOG_INFO(L"%s", str.c_str());

    m_test_peer->send(str.size() * sizeof(WCHAR) + sizeof(WCHAR), str.c_str());
	return true;
}