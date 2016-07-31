#pragma once

#include "EnvironmentManager.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#define FUNCTION_ADD_TEST(no, function)	AddTest(no, value_t(TO_STRING(function), boost::bind(&function)))

class exception_test : public light::EnvironmentManager
{
private:
	typedef int key_t;
	typedef boost::function<void ()> func_t;
	typedef std::pair<std::wstring, func_t> value_t;
	typedef std::map<key_t, value_t> MAP_FUNC;

public:
	exception_test(const std::wstring& program_name);
	~exception_test();

private:
	virtual bool initialize();
	virtual void on_update();
	virtual void release();

	void ProcessInput();

	void Call(value_t& func);

private:
	void AddTest(key_t key, value_t value)
	{
        m_Func.insert({ key, value });
	}

private:
	MAP_FUNC m_Func;
};
