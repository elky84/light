#include "stdafx.h"
#include "Main.h"
#include "ModeManager.h"
#include "TestMode.h"

Main::Main(const std::wstring& strProgramName)
: light::EnvironmentManager(strProgramName)
{
}

Main::~Main()
{
}

bool Main::initialize()
{
	if( FALSE == m_direct_x.Init(get_program_name(), light::view::DEFAULT_FPS, TRUE) )
	{
		LOG_ERROR(L"DirectX::Init() Failed");
		return false;
	}

	if( FALSE == m_direct_x.LoadFont(L"Arita-Medium.ttf"))
	{
		LOG_ERROR(L"DirectX::LoadFont() Failed");
		return false;
	}

	m_direct_x.RegisterMode(new TestMode(&m_direct_x));
	m_direct_x.ActiveMode(0);

	on_complete();
	return true;
}

void Main::on_update()
{
	m_direct_x.on_update();
}

void Main::release()
{
}
