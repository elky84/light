#include "stdafx.h"
#include "remote_server.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring arg;
	for(int i = 0; i < argc; ++i)
		arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(remote_server, L"remote_server", arg);
	return 0;
}
