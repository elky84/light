#pragma once

#include "EnvironmentManager.h"
#include "worker.h"
#include "task.h"

class TaskTest : public light::task
{
public:
	virtual bool on_update()
	{
		LOG_INFO(_T("%s"), __FUNCTIONW__);
		return true;
	}
};

class worker_test 
    : public light::EnvironmentManager
{
public:
	worker_test(const std::wstring& program_name);
	~worker_test();

private:
	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
    light::Thread m_thread;

	light::worker_thread< TaskTest > m_worker_thread;
};
