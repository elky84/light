#pragma once

#include "async_worker.h"
#include "timer.h"

namespace light
{

class timed_worker
{
	typedef std::list< std::shared_ptr<timer> > timer_list;


public:
	timed_worker(void);
	~timed_worker(void);

public:
	bool initialize(void);
	void release(void);

public:
	template<typename Handler>
	void add(boost::asio::io_service& service, UINT interval, Handler handler, const std::string& name)
	{
		std::shared_ptr<timer> timer(new timer(service, interval, handler, name));
		m_timers.push_back(timer);
        timer->start(true);
	}

	template<typename Handler>
	bool remove(Handler handler)
	{
		timer_list::iterator it = find(handler);
		if( it == m_timers.end() )
		{
			LOG_ERROR(L"Cannot find timer object");
			return false;
		}
			
		timer* timer = it->get();
        timer->stop();

		m_timers.erase(it);
		return true;
	}

	///< stop timed callback handler
	template<typename Handler>
	bool stop(Handler handler)
	{
		timer_list::iterator it = find(handler);
		if( it == m_timers.end() )
		{
			LOG_ERROR(L"Cannot find timer object");
			return false;
		}

		timer* timer = it->get();
        timer->stop();
		return true;
	}

	///< resume timed callback handler
	template<typename Handler>
	bool resume(Handler handler)
	{
		timer_list::iterator it = find(handler);
		if( it == m_timers.end() )
		{
            LOG_ERROR(L"Cannot find timer object");
			return false;
		}

		timer* timer = it->get();
        timer->start(true);
		return true;
	}

	void remove_all(void);

	void stop_all(void);

private:

    template<typename Handler>
    timer_list::iterator find(Handler handler)
    {
        return std::find_if(m_timers.begin(), m_timers.end(), [&](auto it) { return handler == it->callback(); });
    }

private:
	timer_list m_timers;
};

} // namespace light