#include "stdafx.h"
#include "test_client.h"

int _tmain(int argc, _TCHAR* argv[])
{
    std::wstring arg;
    for (int i = 0; i < argc; ++i)
        arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(test_client, L"test_client", arg);
	return 0;
}

