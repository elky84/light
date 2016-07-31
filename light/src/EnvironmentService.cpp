#include "light.h"
#include "EnvironmentService.h"
#include "EnvironmentManager.h"

#include "Seh.h"

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	light::EnvironmentService::Instance()->ServiceMain(argc, argv);
}

VOID WINAPI ServiceHandler(DWORD fdwControl)
{
	light::EnvironmentService::Instance()->ServiceHandler(fdwControl);
}

namespace light
{

EnvironmentService::EnvironmentService()
: m_pause(false)
, m_hSrv(NULL)

{
}

bool EnvironmentService::initialize()
{
	if( false == EnvironmentManager::Instance()->initialize() )
	{
		EnvironmentManager::Instance()->stop();
		return false;
	}

	SERVICE_TABLE_ENTRY ste[]=
	{
		{const_cast<WCHAR*>(EnvironmentManager::Instance()->get_program_name()), ::ServiceMain},
		{NULL,NULL}
	};

	BOOL ret = StartServiceCtrlDispatcher(ste);
	if( FALSE == ret )
	{
		LOG_ERROR(L"StartServiceCtrlDispatcher() failed. [%u]", GetLastError());
		EnvironmentManager::Instance()->stop();
		return false;
	}

	return true;
}

void EnvironmentService::release()
{
	EnvironmentManager::Instance()->release();
}

void EnvironmentService::on_update()
{
	while(EnvironmentManager::Instance()->is_active())
	{
		if( false == m_pause ) // pause 상태면 로직 수행 안함. 근데 다른 스레드는?
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

void EnvironmentService::on_complete()
{
	SetStatus(SERVICE_RUNNING);

	on_update();
}

bool EnvironmentService::ServiceMain(DWORD argc, LPTSTR* argv)
{
	LOG_INFO(L"[%d, %s]", argc, argv);

	m_hSrv = RegisterServiceCtrlHandler(EnvironmentManager::Instance()->get_program_name(), ::ServiceHandler);
	if( NULL == m_hSrv )
	{
		LOG_ERROR(L"RegisterServiceCtrlHandler failed. %u", GetLastError());
		EnvironmentManager::Instance()->stop();
		return false;
	}

	// 서비스가 시작중임을 알린다.
	SetStatus(SERVICE_START_PENDING);

	initialize();
	return true;
}

bool EnvironmentService::SetStatus(DWORD dwState, DWORD dwAccept)
{
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = dwState;
	ss.dwControlsAccepted=dwAccept;
	ss.dwWin32ExitCode=0;
	ss.dwServiceSpecificExitCode=0;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=0;

	m_NowState = dwState;
	if(m_hSrv == NULL)
	{
		LOG_ERROR(L"SetStatus Fail. m_hSrv == NULL", dwState);
		return false;
	}

	if( FALSE == SetServiceStatus(m_hSrv, &ss))
	{
		LOG_ERROR(L"SetServiceStatus Fail. %d", dwState);
		return false;
	}

	LOG_INFO(L"SetServiceStatus Success. %d", dwState);
	return true;
}

bool EnvironmentService::ServiceHandler(DWORD fdwControl)
{
	if(fdwControl == m_NowState)
		return false;

	switch(fdwControl)
	{
	case SERVICE_CONTROL_PAUSE:
		SetStatus(SERVICE_PAUSE_PENDING);
		m_pause = true;
		SetStatus(SERVICE_PAUSED);
		return true;
	case SERVICE_CONTROL_CONTINUE:
		SetStatus(SERVICE_CONTINUE_PENDING);
		m_pause = false;
		SetStatus(SERVICE_RUNNING);
		return true;
	case SERVICE_CONTROL_STOP:
		SetStatus(SERVICE_STOP_PENDING);
		EnvironmentManager::Instance()->stop();
		return true;
	case SERVICE_CONTROL_INTERROGATE:
		return false;
	default:
		SetStatus(m_NowState);
		return true;
	}
}


} //namespace light
