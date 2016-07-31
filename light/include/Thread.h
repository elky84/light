#pragma once

#include <memory>
#include <atomic>
#include <boost/function.hpp>

#pragma warning ( push )
#pragma warning ( disable:4100 ) 
#include <boost/thread.hpp>
#pragma warning ( pop )

namespace light
{

class Thread
{
public:
    typedef boost::function< void() > callback;

public:
    Thread(callback update, callback begin = NULL, callback end = NULL);
	~Thread();

    void start();

	void stop();

    void wrapped_update();

    void join();

    bool is_active()
    {
        return m_active.load();
    }

private:
    std::unique_ptr< boost::thread > m_thread;

    boost::function< void() > m_begin;
    boost::function< void() > m_end;

    boost::function< void() > m_update;

    std::atomic<bool> m_active;
};

} //namespace light
