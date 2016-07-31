#pragma once

namespace light
{

class EnvironmentInterface
{
public:
	EnvironmentInterface()
	{
	}

	virtual ~EnvironmentInterface()
	{
	}

	virtual bool initialize() = 0;

	virtual void release() = 0;

	virtual void on_update() = 0;

	virtual void on_complete() = 0;
};

} //namespace light
