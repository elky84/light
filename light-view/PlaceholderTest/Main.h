#pragma once

#include "EnvironmentDirectX.h"
#include "EnvironmentManager.h"

#include "Direct2D.h"

class Main : public light::EnvironmentManager
{
public:
	Main(const std::wstring& strProgramName);
	virtual ~Main();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	light::view::Direct2D m_direct_x;
};
