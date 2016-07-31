#include "stdafx.h"
#include "Session.h"

#include "IocpContext.h"
#include "IocpInterface.h"

#include <memory>

class MockSession : public light::net::Session
{
public:
    MockSession(int index)
        : light::net::Session(index)
    {

    }

    ~MockSession()
    {
    }

    virtual bool send() {
        return m_receive_context->write(m_send_context->get_length(), m_send_context->get_head_buffer());
    }

    virtual bool receive() {
        return m_receive_context->get_length() == m_send_context->get_length();
    }

    virtual bool send(light::net::PACKET_SIZE len, const void* ptr)
    {
        return light::net::Session::send(len, ptr);
    }
};

class Session_Fixture : public testing::Test
{
protected:
    static const int PORT = 1997;

public:
    virtual void SetUp(void)
    {
        light::net::IocpInterface::initialize();
        m_session.reset(new MockSession(1));
    }

    virtual void TearDown(void)
    {
        light::net::IocpInterface::release();
    }

protected:
    std::unique_ptr<MockSession> m_session;
};


TEST_F(Session_Fixture, disconnect)
{
    ASSERT_TRUE(m_session->disconnect() == TRUE);
    ASSERT_TRUE(m_session->disconnect() == FALSE);
}


TEST_F(Session_Fixture, send_receive)
{
    m_session->initialize();
    m_session->set_connected(true);

    int n = 6464;
    ASSERT_TRUE(m_session->send(sizeof(int), &n));

    ASSERT_TRUE(m_session->receive());

}

