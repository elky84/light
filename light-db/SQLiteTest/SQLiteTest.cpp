#include "stdafx.h"
#include "Main.h"
#include "ServiceInstaller.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	RUN_MAIN_CONSOLE(Main, L"SQLiteTest");
#else
	if(argc <= 1)
	{
		RUN_MAIN_SERVICE(Main, L"SQLiteTest");
	}
	else
	{
		std::wstring strArgv = argv[1];
		if(strArgv == L"install")
		{
			SERVICE_INSTALL_AUTOSTART(L"SQLiteTest");
		}
		else if(strArgv == L"uninstall")
		{
			SERVICE_UNINSTALL(L"SQLiteTest");
		}
		else
		{
			wprintf(L"Invalid Command. %s\n", strArgv.c_str() );
		}
	}
#endif
	return 0;
}
