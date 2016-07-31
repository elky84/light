#pragma once

#include "TimeUtil.h"

namespace light
{

class BuildTime
{
public:
	static bool GetTime(SYSTEMTIME& st)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;
		HANDLE hFMap = NULL;
		HMODULE hMpaView = NULL;
		HMODULE hLib = NULL;

		__try
		{
			hLib = LoadLibrary(_T("DbgHelp.dll"));
			if (hLib == NULL)
				return false;

			typedef DWORD (WINAPI *PFTIMESTAMP) (HMODULE);
			PFTIMESTAMP pfTime = (PFTIMESTAMP)GetProcAddress(hLib, "GetTimestampForLoadedLibrary");
			if (pfTime == NULL)
				return false;

			TCHAR szFileName[MAX_PATH] = {0, };
			if (::GetModuleFileName(NULL, szFileName, MAX_PATH) == 0)
				return false;

			hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				return false;

			hFMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			if (hFMap == NULL)
				return false;

			hMpaView = (HMODULE)MapViewOfFile(hFMap, FILE_MAP_READ, 0, 0, 0);
			if (hMpaView == NULL)
				return false;

			DWORD dwTime = pfTime(hMpaView);
			if ( dwTime != 0 )
				return false;

			FILETIME ft;
			TimeUtil::UnixTimeToFileTime(dwTime, ft);
			return TimeUtil::FileTimeToKRLocalTime(ft, st);
		}
		__finally
		{
			if (hLib)
				FreeLibrary(hLib);

			if (hMpaView)
				UnmapViewOfFile(hMpaView);

			if (hFMap)
				CloseHandle(hFMap);

			if (hFile != INVALID_HANDLE_VALUE)
				CloseHandle(hFile);
		}
	}

	static void DisplayConsoleTitle()
	{
		TCHAR szOldTitle[MAX_PATH];
		if (GetConsoleTitle(szOldTitle, MAX_PATH))
		{
			TCHAR szNewTitle[MAX_PATH];
			SYSTEMTIME st;

			if (BuildTime::GetTime(st))
			{
				_stprintf_s(szNewTitle, _T("%d-%02d-%02d %02d:%02d:%02d - %s"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, szOldTitle);
				SetConsoleTitle(szNewTitle);
			}
		}
	}
};

} // namespace light