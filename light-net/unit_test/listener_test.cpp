#include "stdafx.h"
#include "Listener.h"

#include <memory>

#include "SessionManager.h"
#include "Session.h"

class MockPeer 
    : public light::net::PeerEx
{
public:
    MockPeer(const int& idx)
        : light::net::PeerEx(idx)
    {
    }

    ~MockPeer()
    {
    }

    virtual void on_connect()
    {
    }

    virtual void on_disconnect()
    {
    }

    virtual void on_update()
    {
    }

    virtual bool send(light::net::PACKET_SIZE len, const void* ptr)
    {
        UNREFERENCED_PARAMETER(len);
        UNREFERENCED_PARAMETER(ptr);
        return true;
    }

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr)
    {
        UNREFERENCED_PARAMETER(len);
        UNREFERENCED_PARAMETER(ptr);
    }

public:
    static int send_count;
};

int MockPeer::send_count = 0;

class MockListener : public light::net::Listener
{
public:
    MockListener()
        : light::net::Listener(boost::bind(&MockListener::peer_allocator, this, _1))
    {
    }

    ~MockListener()
    {
    }

    light::net::PeerEx* peer_allocator(int idx)
    {
        return new MockPeer(idx);
    }
};

class Listener_Fixture : public testing::Test
{
protected:
    static const int PORT = 1997;

public:
    virtual void SetUp(void)
    {
        light::net::Listener::create_thread(1);
        m_listener.reset(new MockListener());
    }

    virtual void TearDown(void)
    {
        m_listener->stop();
        light::net::Listener::delete_thread();
    }

protected:
    std::unique_ptr<MockListener> m_listener;
};


TEST_F(Listener_Fixture, send)
{
    ASSERT_TRUE(m_listener->start(PORT));

    int n = 6464;
    ASSERT_TRUE(m_listener->send(1, sizeof(int), &n));

}