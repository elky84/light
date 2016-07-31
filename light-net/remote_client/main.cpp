#include "stdafx.h"
#include "remote_client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring arg;
	for(int i = 0; i < argc; ++i)
		arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(remote_client, L"remote_client", arg);
	return 0;
}
