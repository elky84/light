#pragma once

#include "EnvironmentDirectX.h"
#include "EnvironmentManager.h"
#include "Direct3D.h"

/**
@brief Main
@desc Author: Elky
Date: 
*/
class Main : public light::EnvironmentManager
{
public:
	Main(const std::wstring& strProgramName);
	~Main();

private:
	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	light::view::Direct3D m_direct_x;
};
