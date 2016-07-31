#include "stdafx.h"
#include "p2p_test.h"

int _tmain(int argc, WCHAR* argv[])
{
    std::wstring arg;
    for (int i = 0; i < argc; ++i)
        arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(p2p_test, L"p2p_test", arg);
	return 0;
}

