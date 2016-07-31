#include "stdafx.h"
#include "RTTITypeID.hpp"
#include "Factory.hpp"

struct FactoryStruct
{
};

class FactoryFixture : public testing::Test
{
protected:
	typedef light::Factory<light::RTTITypeID, void> Factory;

public:
	virtual void SetUp(void)
	{
	}

	virtual void TearDown(void)
	{
	}
};

TEST_F(FactoryFixture, FactoryStruct)
{
	Factory::Singleton().Support<FactoryStruct>();

	void* factoryPtr = Factory::Singleton().Create<FactoryStruct>();
	ASSERT_TRUE(factoryPtr != NULL);
	SAFE_DELETE(factoryPtr);
}

TEST_F(FactoryFixture, Int)
{
	void* intPtr = Factory::Singleton().Create<int>();
	ASSERT_TRUE(intPtr == NULL);
}