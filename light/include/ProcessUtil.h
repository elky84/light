#pragma once

#include "Util.h"
#include <memory>

namespace light
{

class Process
{
public:
	Process(const std::wstring& ApplicationName, const LPSECURITY_ATTRIBUTES process_security_attributes = NULL, const LPSECURITY_ATTRIBUTES thread_security_attributes = NULL);

    ~Process();

public:
	BOOL create(WCHAR* commandline = NULL);

	BOOL observe();

	void release();

	bool kill();

	static bool execute(const std::wstring& filename, const std::wstring& param = L"", const bool wait = true);

	static HWND pid_to_hwnd(DWORD dwPid);

	static HANDLE find_process(const std::wstring& app_path);

	static HWND find_process_by_name(const std::wstring& class_name, const std::wstring& window_name);

private:
	std::wstring m_appication_path;

	std::wstring m_current_directory;

	std::unique_ptr<SECURITY_ATTRIBUTES> m_process_security_attributes;
	
	std::unique_ptr<SECURITY_ATTRIBUTES> m_thread_security_attributes;
	
	STRUCT_MEMSET_THIS<STARTUPINFO> m_startup_info;

	HANDLE m_process;
};

} //namespace light
