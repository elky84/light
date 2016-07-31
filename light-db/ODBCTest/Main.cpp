#include "stdafx.h"
#include "Main.h"
#include "DBCommand.h"

Main::Main(const std::wstring& strProgramName)
: light::EnvironmentManager(strProgramName)
{
}

Main::~Main()
{
}

bool Main::initialize()
{
	LOG_INFO(L"Start %s Success", get_program_name());

	light::db::Odbc::ACCOUNT_INFO info(L"TEST", L"sa", L"dev");
	m_Worker.push(new TestDBCommand(info));

	on_complete();
	return true;
}

void Main::on_update()
{
	Sleep(1);
	
	if( false == m_Worker.on_update() ) 
	{
		stop();
		LOG_INFO(L"All Task Process."); 
	}
}

void Main::release()
{
}
