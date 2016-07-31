#include "light.h"

#include "timer.h"

#include <boost/bind.hpp>
#pragma warning(push)
#pragma warning(disable: 4127)
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning(pop)

#include <boost/function_types/function_type.hpp>

namespace light
{

timer::timer(boost::asio::io_service& ios)
	: m_io_service(ios)
	, m_timer(ios, boost::posix_time::milliseconds(0))
	, m_interval(0)
	, m_repeat(false)
	, m_suspend(false)
	, m_callback(NULL)
	, m_name()
	, m_suspend_lock()
{
}

timer::timer(boost::asio::io_service& ios, 
			 unsigned int interval, 
			 timer::callback_func m_callback,
			 const std::string& name
			 )
	: m_io_service(ios)
	, m_timer(ios, boost::posix_time::milliseconds(interval))
	, m_interval(interval)
	, m_repeat(false)
	, m_suspend(false)
	, m_callback(m_callback)
	, m_name(name)
	, m_suspend_lock()
{
}

timer::~timer()
{
}

void timer::init(unsigned int interval, timer::callback_func callback)
{
	m_interval = interval;
	m_callback = callback;
}

void timer::start(bool repeat)
{
	m_repeat = repeat;
	if (m_suspend == true)
	{
		boost::mutex::scoped_lock lock(m_suspend_lock);
		m_suspend = false;
	}

	m_timer.expires_from_now(boost::posix_time::milliseconds(m_interval));
	m_timer.async_wait(boost::bind(&timer::on_timer, this, _1));
}

void timer::suspend()
{
	boost::mutex::scoped_lock lock(m_suspend_lock);
	m_suspend = true;
}

// stop method 호출시 timer 객체는 파괴 되지 않지만
// io_service.reset이 불러지기 전까지는 재사용이 불가능하다.
// 만약 재사용 하려면 suspend 호출 후 start를 사용해라.
void timer::stop()
{	 
	m_timer.cancel();
	m_callback = 0;
}

void timer::on_timer(boost::system::error_code const &ec)
{
	if (ec == boost::asio::error::operation_aborted)
		return;

	if (m_callback)
		m_callback();

	boost::mutex::scoped_lock lock(m_suspend_lock);
	if (m_repeat == true && m_suspend == false)
	{
		m_timer.expires_from_now(boost::posix_time::milliseconds(m_interval));
		m_timer.async_wait(boost::bind(&timer::on_timer, this, _1));
	}
}

timer::callback_func timer::callback(void) const
{
	return m_callback;
}

} // namespace light