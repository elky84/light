#include "light.h"
#include "EnvironmentConsole.h"
#include "EnvironmentManager.h"
#include "ConsoleUtil.h"
#include "Seh.h"
#include "LogHelper.h"

#include "ConsoleUtil.h"
#include "TimeUtil.h"

#include <boost/bind.hpp>
#include <fcntl.h>
#include <io.h>

namespace light
{

void console_output(const DWORD thread_id, const DWORD log_level, const std::wstring& get_name, const std::wstring& str)
{
	std::wcout << TimeUtil::get_timestamp().c_str() << L"\t" << thread_id << L"\t" << log_level << L"\t" << get_name << L"\t" << str << std::endl;
}

static bool bind_console()
{
	int consoleHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	if ( consoleHandle == -1 )
	{
		return false;
	}

	FILE* fileHandle = _fdopen(consoleHandle, "w");
	if ( fileHandle == NULL )
	{
		return false;
	}

	*stdout = *fileHandle;
	if ( setvbuf(stdout, NULL, _IONBF, 0) != 0 )
	{
		return false;
	}

#ifdef _DEBUG
#else // _DEBUG
    ConsoleUtil::install_console_ctrl_handler();
    ConsoleUtil::remove_close_button();
#endif // _DEBUG

	return true;
}

bool EnvironmentConsole::console_mode(bool flag)
{
	if(flag)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if( INVALID_HANDLE_VALUE == handle )
		{
			LOG_ERROR(L"GetStdHandle(STD_OUTPUT_HANDLE) failed");
			return false;
		}

		if( NULL == handle )
		{
			if ( AllocConsole() == FALSE )
			{
				LOG_ERROR(L"AllocConsole failed [%d]", GetLastError());
				Seh::RaiseException(__FUNCTION__, 0, 0);
			}
		}

		if( false == bind_console() )
		{
			LOG_ERROR(L"bind_console failed [%d]", GetLastError());
			Seh::RaiseException(__FUNCTION__, 0, 0);
			return false;
		}

		return true;
	}
	else
	{
		if( INVALID_HANDLE_VALUE == GetStdHandle(STD_OUTPUT_HANDLE) )
		{
			LOG_ERROR(L"console_mode turn off. Already STD_OUTPUT_HANDLE is free.");
			return false;
		}

		if( FALSE == ::FreeConsole())
		{
			LOG_ERROR(L"FreeConsole Failed [%d]", GetLastError());
			Seh::RaiseException(__FUNCTION__, 0, 0);
			return false;
		}

		return true;
	}
}

bool EnvironmentConsole::initialize()
{
	ConsoleUtil::set_console_title(EnvironmentManager::Instance()->get_program_name());

	console_mode(true);

	if( false == EnvironmentManager::Instance()->initialize() )
	{
		EnvironmentManager::Instance()->stop();
		return false;
	}

	return true;
}

void EnvironmentConsole::release()
{
	EnvironmentManager::Instance()->release();
}

void EnvironmentConsole::on_update()
{
	while(EnvironmentManager::Instance()->is_active())
	{
		SEH_TRY
		{
			EnvironmentManager::Instance()->on_update();
		}
		SEH_CATCH
		{
		}
	}	
	release();
}

void EnvironmentConsole::on_complete()
{
	on_update();
}

} //namespace light
