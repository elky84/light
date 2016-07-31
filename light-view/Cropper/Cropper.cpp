#include "stdafx.h"
#include "Main.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring strArg;
	for(int i = 0; i < argc; ++i)
		strArg = strArg + L" " + argv[i];

	RUN_MAIN_CONSOLE_ARG(Main, L"Cropper", strArg);
	return 0;
}
