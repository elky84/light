#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#pragma warning(push)
#pragma warning(disable: 4100)
#include <boost/thread.hpp>
#pragma warning(pop)

namespace light
{

class timer
{
public:
	typedef boost::function<void()> callback_func;

public:
	explicit timer(boost::asio::io_service& ios);

	explicit timer(boost::asio::io_service& ios, unsigned int interval, callback_func callback, const std::string& name);

	~timer();

public:
	void init(unsigned int interval, callback_func m_callback);

	void start(bool repeat = false);

	void suspend();

	void stop();

	void on_timer(boost::system::error_code const &ec);

	inline bool suspended(void)
	{ 
		boost::mutex::scoped_lock lock(m_suspend_lock);
		return m_suspend; 
	}

public:
	timer::callback_func callback(void) const;

private:
	boost::asio::io_service& m_io_service;

	boost::asio::deadline_timer m_timer;
	unsigned int m_interval;
	callback_func m_callback;
	bool m_repeat;
	bool m_suspend;
	std::string m_name;
	boost::mutex m_suspend_lock;
};

typedef boost::shared_ptr<timer> timer_ptr;

} // namespace light