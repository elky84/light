#include "light-view.h"

#include "EnvironmentDirectX.h"
#include "EnvironmentManager.h"
#include "WindowUtil.h"

#include "Seh.h"
#include "BgmPlayer.h"

namespace light
{

namespace view
{

EnvironmentDirectX::EnvironmentDirectX(const std::wstring& strWindowClassName, const RECT& rect)
: m_strWindowClassName(strWindowClassName)
, m_hWnd(NULL)
, m_rect(rect)
, m_BgmPlayer(new BgmPlayer)
{
}

EnvironmentDirectX::~EnvironmentDirectX()
{
	m_BgmPlayer.reset();
	FmodEx::Finalize();
}

bool EnvironmentDirectX::initialize()
{
	m_hWnd = light::WindowUtil::CreateMainWindow(light::EnvironmentManager::Instance()->get_program_name(), m_strWindowClassName.c_str(), m_rect, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW);
	if( NULL == m_hWnd )
	{
		light::EnvironmentManager::Instance()->stop();
		return false;
	}

	if( false == FmodEx::Initialize() )
	{
		light::EnvironmentManager::Instance()->stop();
		return false;
	}

	light::WindowUtil::MoveWindowToCenter(m_hWnd);

	light::WindowUtil::Show(m_hWnd);
	
	if( false == light::EnvironmentManager::Instance()->initialize() )
	{
		light::EnvironmentManager::Instance()->stop();
		return false;
	}
	return true;
}

void EnvironmentDirectX::release()
{
	light::EnvironmentManager::Instance()->release();
}

void EnvironmentDirectX::on_update()
{
	while ( light::EnvironmentManager::Instance()->is_active() )
	{
		SEH_TRY
		{
			light::EnvironmentManager::Instance()->on_update();
			FmodEx::System()->update();
		}
		SEH_CATCH
		{

		}
	}

	release();
}

void EnvironmentDirectX::on_complete()
{
	on_update();
}

} // namespace view

} // namespace view
