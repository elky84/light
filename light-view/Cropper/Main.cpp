#include "stdafx.h"
#include "Main.h"
#include "CommandLineArguments.h"

Main::Main(const std::wstring& strProgramName, const std::wstring& strArg)
	: light::EnvironmentManager(strProgramName, strArg)
{
}

Main::~Main()
{
}

bool Main::initialize()
{
	if ( false == m_Crop.ParseCmdLine() )
		return false;

	m_Crop.Slice();

	std::wstring format = light::CommandLineArguments::to_str(L"FORMAT");
	if(format == L"SPR")
	{
		if ( false == m_Crop.WriteSpr() )
			return false;
	}
	else if(format == L"ANI")
	{
		if ( false == m_Crop.WriteAni() )
			return false;
	}
	else
	{
		LOG_ERROR(L"Invalid Format. %s", format.c_str());
		return false;
	}

	LOG_INFO(L"Code Generated Success");
	stop();
	return true;
}

void Main::on_update()
{
}

void Main::release()
{
}