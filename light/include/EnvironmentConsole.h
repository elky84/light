#pragma once

#include "EnvironmentInterface.h"

namespace light
{

class EnvironmentConsole : public EnvironmentInterface
{
public:
	virtual bool initialize();

	virtual void release();

	virtual void on_update();

	virtual void on_complete();

public:
	static bool console_mode(bool flag);
};

} //namespace light
