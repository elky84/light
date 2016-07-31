#include "stdafx.h"
#include "relay_server.h"

#include "boost/bind.hpp"

relay_server::relay_server(const std::wstring& program_name, const std::wstring& arg) 
	: EnvironmentManager(program_name, arg)
{
}

relay_server::~relay_server()
{
	
}

bool relay_server::initialize()
{
	if( false == m_Config.Load() )
	{
		return false;
	}

	light::net::Listener::create_thread(m_Config.thread_count());
	m_Listener.reset(new light::net::Listener(boost::bind(&relay_server::peer_allocator, this, _1)));
	if( false == m_Listener->start(m_Config.Port()) )
	{
		stop();
		return false;
	}

	on_complete();
	return true;
}

void relay_server::on_update()
{
	Sleep(100);

	m_Listener->on_update();
}

void relay_server::release()
{
	light::net::Listener::delete_thread();
	
	stop();
}
