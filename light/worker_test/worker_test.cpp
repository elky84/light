#include "stdafx.h"
#include "worker_test.h"

worker_test::worker_test(const std::wstring& program_name)
: light::EnvironmentManager(program_name)
, m_thread(boost::bind(&worker_test::on_update, this))
{
}

worker_test::~worker_test()
{
}

bool worker_test::initialize()
{
	m_thread.start();
    m_worker_thread.start();

	on_complete();
	return true;
}

void worker_test::on_update()
{
    m_worker_thread.push(new TaskTest);
}

void worker_test::release()
{
    m_worker_thread.stop();
    m_thread.stop();
}