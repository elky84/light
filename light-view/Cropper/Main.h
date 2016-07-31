#pragma once

#include "EnvironmentManager.h"
#include "EnvironmentConsole.h"

#include "Position.h"

#include "Crop.h"

namespace light
{
	class XMLWriter;
} // namespace view

class Main : public light::EnvironmentManager
{
public:
	Main(const std::wstring& strProgramName, const std::wstring& strArg);
	virtual ~Main();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	light::view::Crop m_Crop;
};
