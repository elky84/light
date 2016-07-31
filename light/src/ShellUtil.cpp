#include "light.h"
#include "ShellUtil.h"

#include <ShellAPI.h>
#include "shlwapi.h"
#include <Psapi.h>

#pragma comment( lib, "psapi" )

int ShellUtil::execute(const std::wstring& strCmd, OUT std::wstring& strStream)
{
	FILE* pipe = _tpopen(strCmd.c_str(), L"rt");
	if ( NULL == pipe)
	{
		strStream = L"_popen() execute Failed. Cmd:" + strCmd;
		LOG_ERROR(L"%s", strCmd);
		return -1;
	}

	WCHAR buffer[128] = {0, };
	while(_fgetts(buffer, 128, pipe))
	{
		strStream = strStream + buffer + L"\r\n";
	}

	return _pclose(pipe);
}
