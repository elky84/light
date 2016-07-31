#include "light.h"
#include "ServiceInstaller.h"

namespace light
{

bool ServiceInstaller::Install(const std::wstring& program_name, DWORD dwServiceStartType)
{
    WCHAR path[MAX_PATH] = { 0, };
	::GetModuleFileName( GetModuleHandle(NULL), path, MAX_PATH );

	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if( NULL == hScm )
	{
		LOG_ERROR(L"Not Opened SCM");
		return false;
	}

	SC_HANDLE hSrv = CreateService(hScm, program_name.c_str(), program_name.c_str(), SERVICE_PAUSE_CONTINUE|SERVICE_CHANGE_CONFIG, SERVICE_WIN32_OWN_PROCESS, dwServiceStartType, SERVICE_ERROR_IGNORE, path, NULL, NULL, NULL, NULL, NULL);
	LOG_INFO(L"Service Install %s.", NULL == hSrv ? L"Success" : L"Failed");
	CloseServiceHandle(hScm);
	if(hSrv)
	{
		CloseServiceHandle(hSrv);
	}
	return hSrv != NULL;
}

bool ServiceInstaller::Uninstall(const std::wstring& program_name)
{
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if( NULL == hScm )
	{
		LOG_ERROR(L"Not Opened SCM.");
		return false;
	}

	SC_HANDLE hSrv = OpenService(hScm, program_name.c_str(), SERVICE_ALL_ACCESS);
	if( NULL == hSrv )
	{
		CloseServiceHandle(hScm);
		LOG_ERROR(L"Service Not Installed.");
		return false;
	}

	SERVICE_STATUS ss;
	QueryServiceStatus(hSrv, &ss);

	if(ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	BOOL ret = DeleteService(hSrv);
	LOG_INFO(L"Service Delete %s.", ret ? L"Success" : L"Failed");
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
	return ret == TRUE ? true : false;
}

} //namespace light
