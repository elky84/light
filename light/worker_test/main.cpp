#include "stdafx.h"
#include "worker_test.h"
#include "ServiceInstaller.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	RUN_MAIN_CONSOLE(worker_test, _T("worker_test"));
#else
	if(argc <= 1)
	{
		RUN_MAIN_SERVICE(worker_test, _T("worker_test"));
	}
	else
	{
		std::wstring strArgv = argv[1];
		if(strArgv == _T("install"))
			SERVICE_INSTALL_AUTOSTART(_T("worker_test"));
		else if(strArgv == _T("uninstall"))
			SERVICE_UNINSTALL(_T("worker_test"));
	}
#endif
	return 0;
}
