#include "stdafx.h"
#include "redis_publisher.h"

#include <boost/bind.hpp>

redis_publisher::redis_publisher(const std::wstring& program_name)
    : light::EnvironmentManager(program_name)
{

}

redis_publisher::~redis_publisher()
{

}

static const std::string channelName = "unique-redis-channel-name-example";

bool redis_publisher::initialize()
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;

    m_publisher.reset(new RedisAsyncClient(m_io_service));
    m_publisher->asyncConnect(address, port, [&](bool status, const std::string &err)
    {
        if (!status)
        {
            LOG_ERROR(L"Can't connect to to redis. %S", err.c_str());
            stop();
        }
        else
        {
            m_publisher->publish(channelName, "First hello", [&](const RedisValue &) {
                LOG_INFO(L"send First hello");
                m_publisher->publish(channelName, "Last hello", [&](const RedisValue &) {
                    LOG_INFO(L"send Last hello");
                    m_publisher->publish(channelName, "stop", [&](const RedisValue &) {
                        LOG_INFO(L"stopped");
                        m_io_service.stop();
                        stop();
                    });
                });
            });
        }
    });

    m_io_service.run_one();
    on_complete();
    return true;
}

void redis_publisher::on_update()
{
    Sleep(10);
    m_io_service.run();
}

void redis_publisher::release()
{
}