#include "stdafx.h"
#include "redis_subscriber.h"
#include "ServiceInstaller.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    RUN_MAIN_CONSOLE(redis_subscriber, _T("redis_subscriber"));
#else //_DEBUG
    if (argc <= 1)
    {
        RUN_MAIN_SERVICE(dispatch_worker_test, _T("redis_subscriber"));
    }
    else
    {
        std::wstring strArgv = argv[1];
        if (strArgv == _T("install"))
        {
            SERVICE_INSTALL_AUTOSTART(_T("redis_subscriber"));
        }
        else if (strArgv == _T("uninstall"))
        {
            SERVICE_UNINSTALL(_T("redis_subscriber"));
        }
        else
        {
            _tprintf(_T("Invalid Command. %s\n"), strArgv.c_str());
        }
    }
#endif //_DEBUG
    return 0;
}