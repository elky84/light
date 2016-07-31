#pragma once

#include <unordered_map>
#include <boost/function.hpp>

#include "LogHelper.h"

namespace light
{

template<typename _Key, typename... ARGS>
class DispatcherImplement
{
public:
	typedef boost::function<bool (ARGS...)> func_t;
	typedef std::unordered_map<_Key, func_t>	functions;

public:
	bool register_func(const _Key& key, const func_t& func)
	{
		std::pair<functions::iterator, bool> ret = m_funcs.insert(functions::value_type(key, func));
		return ret.second;
	}

	void unregister(const _Key& key)
	{
		m_funcs.erase(key);
	}


	bool call(const _Key& key, ARGS... args)
	{
		const func_t func = find(key);
		if (NULL == func)
		{
			return false;
		}

		return (func)(args...);
	}

private:
	func_t find(const _Key& key) const
	{
		functions::const_iterator iter = m_funcs.find(key);
		if (iter == m_funcs.end())
		{
			return NULL;
		}
		return iter->second;
	}

private:
	functions m_funcs;
};

//----------------------------------------------------------------------------
template<typename _Key, typename... ARGS>
class Dispatcher
{
private:
	typedef DispatcherImplement<_Key, ARGS...> impl_t;
	typedef boost::function<bool (ARGS...)> func_t;

public: 
	Dispatcher() 
        : m_impl(new impl_t())
    {
    }

public:

	bool register_func(const _Key& key, const func_t& func)
    {
	    bool ret = m_impl->register_func(key, func);
	    if( ret == false )
		    LOG_ERROR(L"Duplicated Function. key[%d]", key);
	    return ret;
    }

    void unregister(const _Key& key)
	{
		m_impl->unregister(key);
	}

private: 
	std::shared_ptr<impl_t> m_impl;

public:
	bool call(const _Key& key, ARGS... args)
	{
		return m_impl->call(key, args...);
	}
};

} // namespace light