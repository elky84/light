#pragma once

#include "EnvironmentManager.h"

class redis_test : public light::EnvironmentManager
{
public:
    redis_test(const std::wstring& program_name);
    ~redis_test();

private:
    virtual bool initialize();
    virtual void on_update();
    virtual void release();
};
