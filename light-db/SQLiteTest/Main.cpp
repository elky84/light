#include "stdafx.h"
#include "Main.h"

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
	on_complete();
	return true;
}

void Main::on_update()
{
	Sleep(1000);
}

void Main::release()
{
}
