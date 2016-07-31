#pragma once

#include "Singleton.h"

#include <map>
#include <list>

#include "boost/shared_ptr.hpp"

namespace light
{

template <typename T>
class ChainingManager 
{
public:
	STATIC_SINGLETON(ChainingManager<T>);

private:
	class ChainingObject
	{
	public:
		ChainingObject()
		{
		}

		~ChainingObject()
		{
		}

		void set(T* p)
		{
			m_chainings.push_back(boost::shared_ptr<T>(p));
		}

		void release()
		{
			m_chainings.clear();
		}

		bool start()
		{
			if( m_chainings.empty() )
				return false;

			auto p = m_chainings.front();
			p->start();
			return true;
		}

		bool next()
		{
			if( m_chainings.empty() )
				return false;

			m_chainings.pop_front();
			return start();
		}

		void on_update()
		{
			if( m_chainings.empty() )
				return;

			auto p = m_chainings.front();
			if( true == p->is_load() )
			{
				next();
			}
			else if( true == p->is_timeout())
			{
				p->timeout();
				release();
			}
		}

		size_t size()
		{
			return m_chainings.size();
		}

	public:
        std::list< boost::shared_ptr<T> > m_chainings; // this container is manage to pointer life-cycle.
	};

private:
	typedef void* KEY;

public:
	ChainingManager()
	{
	}

	~ChainingManager()
	{
		release();
	}

	void set(KEY key, T* p)
	{
        object(key).set(p);
	}

	void release(KEY key)
	{
		return object(key).release();
	}

	void release()
	{
		for each(auto& val in m_chainings)
		{
			ChainingObject& obj = const_cast<ChainingObject&>(val.second);
			obj.release();
		}
	}

	bool start(KEY key)
	{
		return object(key).start();
	}

	bool next(KEY key)
	{
		return object(key).next();
	}

	void on_update()
	{
		/// run is all Chaining process.
		for each(auto& val in m_chainings)
		{
			ChainingObject& obj = const_cast<ChainingObject&>(val.second);
			obj.on_update();
		}
	}

	size_t size(KEY key)
	{
		return object(key).size();
	}

private:
	ChainingObject& object(KEY key)
	{
		return m_chainings[key];
	}

public:
    std::map< KEY, ChainingObject > m_chainings; // this container is manage to pointer life-cycle.
};

} // namespace light
