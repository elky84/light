#pragma once

#include "EnvironmentManager.h"
#include "worker.h"
#include "task.h"
#include "Dispatcher.hpp"

template <typename T>
class TaskTest 
	: public light::task_command<T>
{
public:
	virtual T command()
	{
		return 0;
	}
};

class dispatch_worker_test : public light::EnvironmentManager
{
private:
	typedef int COMMAND;
	typedef TaskTest<COMMAND> TASK;
public:
	dispatch_worker_test(const std::wstring& program_name);
	~dispatch_worker_test();

private:
	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	light::dispatch_worker_thread< TASK, COMMAND > m_worker_thread;

private:
	bool callback_test();
};
