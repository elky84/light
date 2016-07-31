#pragma once

#include "EnvironmentInterface.h"

namespace light
{

namespace view
{

class BgmPlayer;

class EnvironmentDirectX : public light::EnvironmentInterface
{
public:
	EnvironmentDirectX(const std::wstring& strWindowClassName, const RECT& rect);

	virtual ~EnvironmentDirectX();

	virtual bool initialize();

	virtual void release();

	virtual void on_update();

	virtual void on_complete();

private:
	std::wstring m_strWindowClassName;

	HWND m_hWnd; 

	const RECT m_rect;

	std::auto_ptr<BgmPlayer> m_BgmPlayer;
};

} // namespace view

} // namespace view

#define RUN_MAIN_DIRECTX(ClassName, szProgramName, szClassName, rect) new ClassName(szProgramName); light::EnvironmentManager::Instance()->setup(new light::view::EnvironmentDirectX(szClassName, rect)); delete light::EnvironmentManager::Instance();
#define RUN_MAIN_DIRECTX_ARG(ClassName, szProgramName, strArg, szClassName, rect) new ClassName(szProgramName, strArg); light::EnvironmentManager::Instance()->setup(new light::view::EnvironmentDirectX(szClassName, rect)); delete light::EnvironmentManager::Instance();
