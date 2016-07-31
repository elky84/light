#include "light.h"
#include "timed_worker.h"

namespace light
{

timed_worker::timed_worker(void)
{
}

timed_worker::~timed_worker(void)
{
}

bool timed_worker::initialize(void)
{
	return true;
}

void timed_worker::release(void)
{
	remove_all();
}

void timed_worker::remove_all(void)
{
	stop_all();

	m_timers.clear();
}

void timed_worker::stop_all(void)
{
	for each (auto timer in m_timers)
	{
        timer->stop();
	}
}

} // namespace light