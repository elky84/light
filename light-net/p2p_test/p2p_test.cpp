#include "stdafx.h"
#include "p2p_test.h"

#include "P2PRelayConnector.h"
#include "IocpInterface.h"

p2p_test::p2p_test(const std::wstring& program_name, const std::wstring& arg)
	: EnvironmentManager(program_name, arg)
{
	m_p2p.reset(new P2P(m_dispatcher));
	m_RelayConnector.reset(new light::net::P2PRelayConnector(m_p2p.get(), m_dispatcher));
	m_p2p->set(m_RelayConnector);
}

p2p_test::~p2p_test()
{
}

bool p2p_test::initialize()
{
	if( false == m_Config.Load())
	{
		stop();
		return false;
	}

	if( false == m_p2p->bind(m_Config.Port()) )
	{	
		stop();
		return false;
	}

	m_RelayConnector->connect(WCHAR_TO_CHAR(m_Config.IP().c_str()), m_Config.Port());



	on_complete();
	return true;
}

void p2p_test::on_update()
{
	Sleep(100);
	m_p2p->on_update();

	if(false == m_RelayConnector->is_connected())
	{
		m_RelayConnector->connect(WCHAR_TO_CHAR(m_Config.IP().c_str()), m_Config.Port());
	}
	else
	{
		m_RelayConnector->on_update();

		static int i = 0;
		++i;

		if(i % 5 == 0)
			m_RelayConnector->enter(1);

		m_RelayConnector->alive_check();
	}

	m_p2p->pop();
	m_RelayConnector->pop();
}

void p2p_test::on_complete()
{
	
}

void p2p_test::release()
{

}
