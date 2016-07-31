#include "stdafx.h"
#include <redisclient/redissyncclient.h>

#include <redisclient/redisasyncclient.h>

TEST(redisclient, get_set)
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;

    boost::asio::io_service ioService;
    RedisSyncClient redis(ioService);
    std::string errmsg;

    ASSERT_TRUE(redis.connect(address, port, errmsg));

    RedisValue result;
    result = redis.command("SET", "key", "value");

    ASSERT_FALSE(result.isError());

    result = redis.command("GET", "key");

    ASSERT_TRUE(result.isOk());
    ASSERT_EQ(result.toString(), "value");
}

static const std::string redisKey = "unique-redis-key-example";
static const std::string redisValue = "unique-redis-value";

void handleConnected(boost::asio::io_service &ioService, RedisAsyncClient &redis,
    bool ok, const std::string &errmsg)
{
    ASSERT_TRUE(ok);
    ASSERT_EQ(errmsg, "");

    redis.command("SET", redisKey, redisValue, [&](const RedisValue &v) 
    {
        ASSERT_EQ(v.toString(), "OK");

        redis.command("GET", redisKey, [&](const RedisValue &v) 
        {
            ASSERT_EQ(v.toString(), redisValue);

            redis.command("DEL", redisKey, [&](const RedisValue &) 
            {
                ioService.stop();
            });
        });
    });
}

TEST(redisclient, async_get_set)
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;

    boost::asio::io_service ioService;
    RedisAsyncClient redis(ioService);

    redis.asyncConnect(address, port, boost::bind(&handleConnected, boost::ref(ioService), boost::ref(redis), _1, _2));

    ioService.run();
}


static const std::string channelName = "unique-redis-channel-name-example";

void subscribeHandler(boost::asio::io_service &ioService, const std::vector<char> &buf)
{
    std::string msg(buf.begin(), buf.end());
    if (msg == "stop")
        ioService.stop();
}

void publishHandler(RedisAsyncClient &publisher, const RedisValue &)
{
    publisher.publish(channelName, "First hello", [&](const RedisValue &) {
        publisher.publish(channelName, "Last hello", [&](const RedisValue &) {
            publisher.publish(channelName, "stop");
        });
    });
}

TEST(redisclient, publish_subscribe)
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;

    boost::asio::io_service ioService;
    RedisAsyncClient publisher(ioService);
    RedisAsyncClient subscriber(ioService);

    publisher.asyncConnect(address, port, [&](bool status, const std::string &err)
    {
        ASSERT_TRUE(status);
        ASSERT_EQ(err, "");

        subscriber.asyncConnect(address, port, [&](bool status, const std::string &err)
        {
            ASSERT_TRUE(status);
            ASSERT_EQ(err, "");

            subscriber.subscribe(channelName,
                boost::bind(&subscribeHandler, boost::ref(ioService), _1),
                boost::bind(&publishHandler, boost::ref(publisher), _1));
        });
    });

    ioService.run();
}