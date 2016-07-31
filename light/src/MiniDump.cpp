#include "light.h"
#include "MiniDump.h"

#include "FileUtil.h"
#include "DirectoryUtil.h"
#include "TimeUtil.h"

namespace light
{

void MiniDumper::Log(EXCEPTION_POINTERS* info)
{
	_declspec (thread) static WCHAR description[8192];
	if (info->ExceptionRecord->NumberParameters >= 3)
	{
		_stprintf_s(description, L"EXCEPTION_WITH_INFO : msg[%S] arg1[%d] arg2[%d]",
			(char*)info->ExceptionRecord->ExceptionInformation[0], info->ExceptionRecord->ExceptionInformation[1], info->ExceptionRecord->ExceptionInformation[2]);
	}
	else
	{
		_stprintf_s(description, L"EXCEPTION_WITH_INFO : invalid parameter count[%d]", info->ExceptionRecord->NumberParameters);
	}

	LOG_ERROR(L"[%s]", description);
}

MiniDumper::MiniDumper()
{	
	memset(m_dump_path, 0, sizeof(m_dump_path));
}

LONG MiniDumper::WriteMiniDump(EXCEPTION_POINTERS *pExceptionInfo)
{
	//정확한 dbghelp.dll을 찾아야 한다.
	//System32에 있는 것이 오래된 것(Win2K)일 수 있으므로, exe가 있는 
	//디렉토리를 먼저 뒤져야 한다. 즉, 배포본에는 따로 DBGHELP.DLL을 
	//포함시켜 디버깅할 수 있다는 얘기.
	m_app_name = get_module_name();
	m_app_path = get_module_path();

	std::wstring dbg_help_path = m_app_path + L"\\dbghelp.dll";

    LOG_INFO(L"app_name[%s] app_path[%s] dbg_help_path[%s]", m_app_name.c_str(), m_app_path.c_str(), dbg_help_path.c_str());

    HMODULE hDll = ::LoadLibrary(dbg_help_path.c_str());
    if(hDll == NULL)
	{
		//못 찾았음. 기본 DLL 로드한다.
		hDll = ::LoadLibrary(L"dbghelp.dll");
	}

	if(hDll == NULL)
	{
		LOG_ERROR(L"dbghelp.dll is not find.");
		return EXCEPTION_CONTINUE_SEARCH;
	}

	MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
	if( NULL == pMiniDumpWriteDump)
	{
		LOG_ERROR(L"dbghelp.dll is Old version.");
		return EXCEPTION_CONTINUE_SEARCH;
	}

	WCHAR szFileName[MAX_PATH] = {0, };
	_stprintf_s(szFileName, L"%s_%u_%s", get_module_name().c_str(), GetCurrentThreadId(), TimeUtil::get_file_timestamp().c_str());
	SetDumpFileName(szFileName);

	//덤프 파일 생성
	HANDLE hFile = ::CreateFile(m_dump_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		LPVOID	pMsg;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsg, 0, NULL);

		LOG_ERROR(L"DumpFile '%s' Create Failed (Error %s)", m_dump_path, pMsg);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

	ExInfo.ThreadId = ::GetCurrentThreadId();
	ExInfo.ExceptionPointers = pExceptionInfo;
	ExInfo.ClientPointers = NULL;

	//덤프 기록
	if( FALSE == pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, GetUserStreamArray(), NULL))
	{
		LPVOID pMsg;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsg, 0, NULL);
		LOG_ERROR(L"DumpFile '%s' Save Failed (Error %s)", m_dump_path, pMsg);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	::CloseHandle(hFile);

	Log(pExceptionInfo);
	return EXCEPTION_EXECUTE_HANDLER;
}

void MiniDumper::SetDumpFileName(const std::wstring& file_name)
{
	_stprintf_s(m_dump_path, L"%s%s.dmp", m_app_path.c_str(), file_name.c_str());
}

} //namespace light
