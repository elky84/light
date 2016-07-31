#include "stdafx.h"
#include "Main.h"

int _tmain(int argc, WCHAR* argv[])
{
	std::wstring strArg;
	if( 1 < argc )
	{
		strArg = argv[1];
	}

	RUN_MAIN_CONSOLE_ARG(Main, L"ExcelToXML", strArg);
	return 0;
}
