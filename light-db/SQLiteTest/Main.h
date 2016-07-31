#pragma once

#include "EnvironmentManager.h"

class Main : public light::EnvironmentManager
{
public:
	Main(const std::wstring& strProgramName);
	~Main();

private:
	virtual bool initialize();
	virtual void on_update();
	virtual void release();
};
