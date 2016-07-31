#pragma once

#include "Singleton.h"

#include "EnvironmentConsole.h"
#include "EnvironmentService.h"
#include "EnvironmentWindow.h"

#include <memory>
#include <atomic>

namespace light
{

class LeakTracking;

class EnvironmentManager : public Singleton<EnvironmentManager>
{
protected:
	EnvironmentManager(const std::wstring& program_name, const std::wstring& arg = L"");

public:
	virtual ~EnvironmentManager();

	void setup(EnvironmentInterface* environment);

    bool is_active()
	{
		return m_active.load();
	}

	const WCHAR* get_program_name()
	{
		return m_program_name.c_str();
	}

    virtual void stop();

	virtual bool initialize() = 0;

	virtual void on_update() = 0;

	virtual void release() = 0;

	void on_complete();

private:
	std::wstring m_program_name;

	std::atomic<bool> m_active;

	std::unique_ptr<EnvironmentInterface> m_environment;

#ifdef _DEBUG
	std::unique_ptr<LeakTracking> m_leak_tracking;
#endif //_DEBUG
};

} //namespace light

#define RUN_MAIN_CONSOLE(ClassName, szProgramName) new ClassName(szProgramName); light::EnvironmentManager::Instance()->setup(new light::EnvironmentConsole); delete light::EnvironmentManager::Instance();
#define RUN_MAIN_WINDOW(ClassName, szProgramName, szClassName, rect) new ClassName(szProgramName); light::EnvironmentManager::Instance()->setup(new light::EnvironmentWindow(szClassName, rect)); delete light::EnvironmentManager::Instance();
#define RUN_MAIN_SERVICE(ClassName, szProgramName) new ClassName(szProgramName); light::EnvironmentManager::Instance()->setup(new light::EnvironmentService); delete light::EnvironmentManager::Instance();

#define RUN_MAIN_CONSOLE_ARG(ClassName, szProgramName, Arg) new ClassName(szProgramName, Arg); light::EnvironmentManager::Instance()->setup(new light::EnvironmentConsole); delete light::EnvironmentManager::Instance();
#define RUN_MAIN_WINDOW_ARG(ClassName, szProgramName, Arg, szClassName, rect) new ClassName(szProgramName, Arg); light::EnvironmentManager::Instance()->setup(new light::EnvironmentWindow(szClassName, rect)); delete light::EnvironmentManager::Instance();
