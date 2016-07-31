#include "stdafx.h"
#include "game_server.h"

int _tmain(int argc, WCHAR* argv[])
{
    std::wstring arg;
    for (int i = 0; i < argc; ++i)
        arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(game_server, L"game_server", arg);
	return 0;
}