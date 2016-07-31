#include "light.h"
#include "EnvironmentWindow.h"
#include "EnvironmentManager.h"
#include "WindowUtil.h"
#include "Seh.h"

namespace light
{

EnvironmentWindow::EnvironmentWindow(const std::wstring& strWindowClassName, const RECT& rect)
: m_WindowClassName(strWindowClassName)
, m_hWnd(NULL)
, m_rect(rect)
{
}

bool EnvironmentWindow::initialize()
{
	m_hWnd = WindowUtil::CreateMainWindow(EnvironmentManager::Instance()->get_program_name(), m_WindowClassName.c_str(), m_rect, WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_APPWINDOW);
	if( NULL == m_hWnd )
	{
		EnvironmentManager::Instance()->stop();
		return false;
	}

	WindowUtil::Show(m_hWnd);

	WindowUtil::SetupCallbacks();

	if( false == EnvironmentManager::Instance()->initialize() )
	{
		EnvironmentManager::Instance()->stop();
		return false;
	}

	return true;
}

void EnvironmentWindow::release()
{
	EnvironmentManager::Instance()->release();
}

void EnvironmentWindow::on_update()
{
	MSG Msg;
	while ( EnvironmentManager::Instance()->is_active() )
	{
		if( TRUE == PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if ( false == WindowUtil::CallDispatcher(Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam) )
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
		else
		{
			SEH_TRY
			{
				EnvironmentManager::Instance()->on_update();
			}
			SEH_CATCH
			{

			}
		}
	}

	release();
}

void EnvironmentWindow::on_complete()
{
	on_update();
}

} //namespace light
