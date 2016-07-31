#pragma once

#include "EnvironmentInterface.h"
#include "Singleton.h"

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

namespace light
{

class EnvironmentService 
	: public EnvironmentInterface
	, public Singleton<EnvironmentService>
{
public:
	EnvironmentService();

	virtual bool initialize();

	virtual void release();

	virtual void on_update();

	virtual void on_complete();

	bool ServiceMain(DWORD argc, LPTSTR* argv);

	bool ServiceHandler(DWORD fdwControl);

private:
	bool SetStatus(DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE);

private:
	DWORD m_NowState;

	bool m_pause;

	SERVICE_STATUS_HANDLE m_hSrv;
};

} //namespace light
