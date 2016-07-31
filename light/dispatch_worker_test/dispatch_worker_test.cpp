#include "stdafx.h"
#include "dispatch_worker_test.h"

#include <boost/bind.hpp>

dispatch_worker_test::dispatch_worker_test(const std::wstring& program_name)
	: light::EnvironmentManager(program_name)
{
}

dispatch_worker_test::~dispatch_worker_test()
{
}

bool dispatch_worker_test::initialize()
{
    m_worker_thread.start();

    m_worker_thread.register_func(0, boost::bind(&dispatch_worker_test::callback_test, this));

    on_complete();
	return true;
}

void dispatch_worker_test::on_update()
{
    m_worker_thread.push(new TASK);
}

void dispatch_worker_test::release()
{
	m_worker_thread.stop();
}

bool dispatch_worker_test::callback_test()
{
	LOG_INFO(_T("%s"), __FUNCTIONW__);
	return true;
}
