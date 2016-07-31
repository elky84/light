#include "stdafx.h"
#include "FSM.h"

#include <boost/bind.hpp>

class FSM_STATE
{
public:
	typedef enum Type
	{
		NONE		= -1,
		ATTACK		= 0,
		MOVING		= 1,
		IDLE		= 2,
		END
	};
};

class FSM_Fixture 
	: public testing::Test
	, public FSM<FSM_STATE::Type>
{
protected:
	virtual void SetUp(void)
	{
	}

	virtual void TearDown(void)
	{
	}

public:
	void Moving()
	{

	}

	void Idle()
	{

	}

	void Attack()
	{

	}
};

TEST_F(FSM_Fixture, Test)
{
	EXPECT_TRUE( AddState(FSM_STATE::IDLE, FSM_STATE::MOVING, boost::bind(&FSM_Fixture::Idle, this), NULL, NULL ));
	EXPECT_TRUE( AddState(FSM_STATE::ATTACK, FSM_STATE::MOVING, boost::bind(&FSM_Fixture::Attack, this), NULL, NULL ));
	EXPECT_TRUE( AddState(FSM_STATE::MOVING, FSM_STATE::IDLE, boost::bind(&FSM_Fixture::Moving, this), NULL, NULL ));
	EXPECT_TRUE( Transit(FSM_STATE::IDLE) );
}