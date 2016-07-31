#include "stdafx.h"

#include "Chaining.h"

#include "boost/bind.hpp"

class Work
{
public:
	Work()
		: m_load(false)
	{

	}

	~Work()
	{

	}

	void on_start()
	{
        m_load = true;
	}

	bool& is_load()
	{
		return m_load;
	}

	void on_timeout()
	{

	}

private:
	bool m_load;
};

class ChainingFixture 
	: public testing::Test
{
protected:
	ChainingFixture(void)
	{
	}

	~ChainingFixture(void)
	{
	}


	virtual void SetUp(void)
	{
		for(int i = 0; i < _countof(m_Work); ++i)
		{
			light::chaining_register(this, new light::Chaining(m_Work[i].is_load(), boost::bind(&Work::on_start, &m_Work[i]), boost::bind(&Work::on_timeout, &m_Work[i])));
		}
	}

	virtual void TearDown(void)
	{
	}

	ChainingFixture* This()
	{
		return this;
	}

protected:
	Work m_Work[5];
};


TEST_F(ChainingFixture, WorkTest)
{
	light::chaining_start<light::Chaining>(This());
	ASSERT_EQ(light::chaining_size<light::Chaining>(This()), _countof(m_Work));

	for(size_t i = light::chaining_size<light::Chaining>(This()); 0 < i; --i)
	{
		ASSERT_EQ(i, light::chaining_size<light::Chaining>(This()));
		light::chaining_update<light::Chaining>();
	}

	ASSERT_EQ(0, light::chaining_size<light::Chaining>(This()));
}