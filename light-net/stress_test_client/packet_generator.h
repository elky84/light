#pragma once

#include "light-net.h"
#include "LinearBuffer.h"

class packet_generator
{
public:
	packet_generator();
	virtual ~packet_generator();

	bool light(light::LinearNormalBuffer* buffer);

    bool heavy(light::LinearNormalBuffer* buffer);

    bool random(light::LinearNormalBuffer* buffer);
};
