#pragma once

#include "Thread.h"
#include "Singleton.h"

#include <boost/function.hpp>

namespace light
{

class PromptInterface 
    : public Singleton<PromptInterface>
{
public:
	typedef boost::function<bool(const std::wstring&)> command_callback;

	typedef boost::function<void()> end_callback;
    typedef boost::function<void()> begin_callback;

public:
	PromptInterface(const end_callback end, const std::wstring& notice = L"")
		: m_notice(notice)
        , m_thread(new Thread(boost::bind(&PromptInterface::on_update, this), boost::bind(&PromptInterface::begin, this), end))
	{
	}

	virtual ~PromptInterface()
	{
	}

    void start()
    {
        m_thread->start();
    }

    virtual void begin() = 0;

    virtual void on_update() = 0;

	virtual void notice(const std::wstring& notice)
	{
		m_notice = notice;
	}

	virtual void stop()
	{
        m_thread->stop();
        m_callback = NULL;
    }

	virtual void callback(const command_callback callback)
	{
		m_callback = callback;
	}

protected:

	virtual bool call()
	{
		if(m_callback == NULL)
		{
			notice(L"Not Defined callback");
			return false;
		}

		return m_callback(m_command);
	}


protected:
	std::wstring m_command;

	std::wstring m_notice;

	command_callback m_callback;

    std::unique_ptr< Thread > m_thread;
};

} //namespace light