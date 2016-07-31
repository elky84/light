#include "stdafx.h"
#include "dispatch_worker_test.h"
#include "ServiceInstaller.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	RUN_MAIN_CONSOLE(dispatch_worker_test, _T("dispatch_worker_test"));
#else //_DEBUG
	if(argc <= 1)
	{
		RUN_MAIN_SERVICE(dispatch_worker_test, _T("dispatch_worker_test"));
	}
	else
	{
		std::wstring strArgv = argv[1];
		if(strArgv == _T("install"))
		{
			SERVICE_INSTALL_AUTOSTART(_T("dispatch_worker_test"));
		}
		else if(strArgv == _T("uninstall"))
		{
			SERVICE_UNINSTALL(_T("dispatch_worker_test"));
		}
		else
		{
			_tprintf(_T("Invalid Command. %s\n"), strArgv.c_str() );
		}
	}
#endif //_DEBUG
	return 0;
}
