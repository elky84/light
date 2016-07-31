#pragma once

#include "EnvironmentInterface.h"

namespace light
{

class EnvironmentWindow : public EnvironmentInterface
{
public:
	EnvironmentWindow(const std::wstring& strWindowClassName, const RECT& rect);

	virtual bool initialize();

	virtual void release();

	virtual void on_update();

	virtual void on_complete();

private:
	std::wstring m_WindowClassName;

	HWND m_hWnd; 

	const RECT m_rect;
};

} //namespace light
