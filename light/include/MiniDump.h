#pragma once

#include "Singleton.h"
#include <dbghelp.h>

namespace light
{

class MiniDumper
{
public:
	STATIC_SINGLETON(MiniDumper);

public:
	MiniDumper();

	LONG WriteMiniDump(EXCEPTION_POINTERS *pExceptionInfo);

protected:
	void SetDumpFileName(const std::wstring& file_name);

	virtual MINIDUMP_USER_STREAM_INFORMATION* GetUserStreamArray()
	{
		return NULL;
	}

private:
	void Log(EXCEPTION_POINTERS* info);

private:
	WCHAR m_dump_path[MAX_PATH];

	std::wstring m_app_path;

	std::wstring m_app_name;
};

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, 
			CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

} //namespace light
