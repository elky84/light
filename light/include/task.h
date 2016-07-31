#pragma once

namespace light
{

class task
{
public:
	task()
	{
	}

	virtual ~task()
	{
	}

	virtual bool on_update() = 0;
};

template <typename T>
class task_command
{
public:
	task_command()
	{
	}

	virtual ~task_command()
	{
	}

	virtual T command() = 0;

	virtual bool on_update()
	{
		return false;
	}
};

} // namespace light
