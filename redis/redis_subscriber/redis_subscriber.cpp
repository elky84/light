#include "stdafx.h"
#include "redis_subscriber.h"

#include <boost/bind.hpp>

redis_subscriber::redis_subscriber(const std::wstring& program_name)
    : light::EnvironmentManager(program_name)
{

}

redis_subscriber::~redis_subscriber()
{

}

static const std::string channelName = "unique-redis-channel-name-example";

void subscribeHandler(boost::asio::io_service &ioService, const std::vector<char> &buf)
{
    std::string msg(buf.begin(), buf.end());
    LOG_INFO(L"%S", msg.c_str());

    if (msg == "stop")
    {
        ioService.stop();
        light::EnvironmentManager::Instance()->stop();
    }
}

bool redis_subscriber::initialize()
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;

    m_subscriber.reset(new RedisAsyncClient(m_io_service));

    m_subscriber->asyncConnect(address, port, [&](bool status, const std::string &err)
    {
        if (!status)
        {
            std::cerr << "Can't connect to to redis" << err << std::endl;
        }
        else
        {
            m_subscriber->subscribe(channelName, boost::bind(&subscribeHandler, boost::ref(m_io_service), _1));
        }
    });

    m_io_service.run_one();
    on_complete();
    return true;
}

void redis_subscriber::on_update()
{
    Sleep(10);
    m_io_service.run_one();
}

void redis_subscriber::release()
{
}