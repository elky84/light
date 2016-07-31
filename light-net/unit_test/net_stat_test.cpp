#include "stdafx.h"
#include "NetStat.h"

#include "Session.h"
#include "IocpInterface.h"


class NetStat_Fixture : public testing::Test
{
protected:
    static const int PORT = 1997;

public:
    virtual void SetUp(void)
    {
        light::net::IocpInterface::initialize();
        m_session.reset(new light::net::Session(1));
    }

    virtual void TearDown(void)
    {
        light::net::IocpInterface::release();
    }

protected:
    std::unique_ptr<light::net::Session> m_session;
};


TEST_F(NetStat_Fixture, NetStat)
{
    ASSERT_TRUE(m_session->disconnect() == TRUE);

    light::net::NetStat::write([](auto str) { ASSERT_NE(str, ""); });
}