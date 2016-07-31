#include "stdafx.h"
#include "Dispatcher.hpp"
#include <boost/bind.hpp>

enum COMMAND
{
	COMMAND_TEST = 0,
};

class Dispatcher_Fixture : public testing::Test
{
public:
	virtual void SetUp()
	{
		m_Dispatcher.register_func(COMMAND_TEST, boost::bind(&Dispatcher_Fixture::Func, this));
	}

	bool Call(COMMAND command)
	{
		return m_Dispatcher.call(command);
	}

private:
	bool Func()
	{
		return true;
	}

private:
	light::Dispatcher<COMMAND> m_Dispatcher;
};

TEST_F(Dispatcher_Fixture, Call)
{
	ASSERT_TRUE(Call(COMMAND_TEST));
}
