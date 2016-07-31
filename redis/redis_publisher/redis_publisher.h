#pragma once

#include "EnvironmentManager.h"
#include <redisclient/redisasyncclient.h>

class redis_publisher : public light::EnvironmentManager
{
public:
    redis_publisher(const std::wstring& program_name);
    ~redis_publisher();

private:
    virtual bool initialize();
    virtual void on_update();
    virtual void release();

private:
    boost::asio::io_service m_io_service;
    std::unique_ptr<RedisAsyncClient> m_publisher;
};
