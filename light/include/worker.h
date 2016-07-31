#pragma once

#include "Thread.h"
#include "DoubleQueue.h"
#include "Dispatcher.hpp"

#include <memory>

namespace light
{

template <typename Task_t>
class worker 
	: public Singleton< worker<Task_t> >
{
protected:
	typedef std::shared_ptr<Task_t> task_ptr;

private:
	typedef DoubleQueue<task_ptr> Queue;

public:
	explicit worker()
	{
	}

	virtual ~worker()
	{
	}

public:
	bool on_update()
	{
		if(m_queue.empty())
			return false;

		task_ptr task;
		while( true == m_queue.pop(task) )
		{
			if ( false == do_work(task) )
			{
				LOG_ERROR(L"Work failed.");
			}
		}

		return true;
	}

	virtual bool do_work(task_ptr task)
	{
		return task->on_update();
	}

	void push(Task_t* task)
	{
		m_queue.push(task_ptr(task));
	}

private:
	Queue m_queue;
};

template <typename Task_t>
class worker_thread 
    : public worker<Task_t>
{
public:
	explicit worker_thread()
        : m_thread(boost::bind(&worker_thread::on_update, this))
	{
	}

	virtual ~worker_thread()
	{
	}

    void start()
    {
        m_thread.start();
    }

    void stop()
    {
        m_thread.stop();
    }

private:
	virtual void on_update()
	{
		worker::on_update();
	}

    Thread m_thread;
};

template <typename Task_t, typename Command_t>
class dispatch_worker_thread : public worker_thread<Task_t>
{
public:
	explicit dispatch_worker_thread()
	{
	}

	virtual ~dispatch_worker_thread()
	{
	}

	template <typename Key_t, typename Function_t>
	bool register_func(const Key_t& key, const Function_t& func)
	{
		return m_dispatcher.register_func(key, func);
	}

	virtual bool do_work(task_ptr task)
	{
		return m_dispatcher.call(task->command(), *task);
	}

private:
	Dispatcher<Command_t, Task_t> m_dispatcher;
};

} // namespace light
