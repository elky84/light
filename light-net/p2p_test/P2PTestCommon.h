#pragma once

#include "P2PCommon.h"

enum P2P_TEST_COMMAND
{ 
	P2PCHAT = 10000,
};

struct P2PChat 
{
	P2PChat() 
	{
	}

public:
	TCHAR STR[512];
};
