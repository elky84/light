#include "stdafx.h"
#include "Connector.h"
#include <memory>

#include "SessionManager.h"
#include "Session.h"
#include "Peer.h"

class MockPeer
    : public light::net::Peer
{
public:
    MockPeer()
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
};


class MockConnector : public light::net::Connector
{
public:
    MockConnector()
        : light::net::Connector(new MockPeer())
    {
    }

    ~MockConnector()
    {
    }

    virtual bool connect()
    {
        m_session->set_connected(true);
        return true;
    }
};

class Connector_Fixture : public testing::Test
{
protected:
    static const int PORT = 1997;

public:
    virtual void SetUp(void)
    {
        light::net::Connector::create_thread(1);
        m_Connector.reset(new MockConnector());
    }

    virtual void TearDown(void)
    {
        m_Connector->stop();
        light::net::Connector::delete_thread();
    }

protected:
    std::unique_ptr<MockConnector> m_Connector;
};


TEST_F(Connector_Fixture, send)
{
    ASSERT_TRUE(m_Connector->connect());

    int n = 6464;
    ASSERT_TRUE(m_Connector->send(sizeof(int), &n));

}