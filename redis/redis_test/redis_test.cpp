#include "stdafx.h"
#include "redis_test.h"

#include <boost/bind.hpp>

redis_test::redis_test(const std::wstring& program_name)
    : light::EnvironmentManager(program_name)
{

}

redis_test::~redis_test()
{

}

bool redis_test::initialize()
{
    on_complete();
    return true;
}

void redis_test::on_update()
{
}

void redis_test::release()
{
}