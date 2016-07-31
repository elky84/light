#pragma once

#include "Str.h"
#include <Windows.h>

namespace light
{

/**
@brief 서비스 컨트롤러
*/
class ServiceInstaller
{
public:
	/// 서비스 설치 메소드
	static bool Install(const std::wstring& program_name, DWORD dwServiceStartType);

	/// 서비스 제거 메소드
	static bool Uninstall(const std::wstring& program_name);
};

} //namespace light

///서비스 자동 시작으로 설치
#define SERVICE_INSTALL_AUTOSTART(szProgramName) light::ServiceInstaller::Install(szProgramName, SERVICE_BOOT_START | SERVICE_AUTO_START)

///서비스 수동 시작으로 설치
#define SERVICE_INSTALL_MANUALSTART(szProgramName) light::ServiceInstaller::Install(szProgramName, SERVICE_DEMAND_START)

///서비스 제거
#define SERVICE_UNINSTALL(szProgramName) light::ServiceInstaller::Uninstall(szProgramName)
