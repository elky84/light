#include "stdafx.h"
#include "chat_client.h"

int _tmain(int argc, WCHAR* argv[])
{
    std::wstring arg;
    for (int i = 0; i < argc; ++i)
        arg = arg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(chat_client, _T("chat_client"), arg);
	return 0;
}

