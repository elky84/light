#include "stdafx.h"
#include "smart_singleton.h"

struct SmartSingletonTest
{
public:
	SmartSingletonTest()
	{
	}

	~SmartSingletonTest()
	{
	}
};

TEST(SmartSingleton, Instance)
{
	{
		light::smart_singleton<SmartSingletonTest> Singleton;
		ASSERT_TRUE(light::smart_singleton<SmartSingletonTest>::smart_instance() != NULL);
	}

	{
		ASSERT_TRUE(light::smart_singleton<SmartSingletonTest>::smart_instance() == NULL);
	}
}
