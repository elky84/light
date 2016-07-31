#include "stdafx.h"
#include "timed_worker.h"

class timed_worker_Fixture : public testing::Test
{
public:
    int call_count() const
    {
        return m_call_count;
    }

    void test()
    {
        m_call_count++;
    }

private:
    int m_call_count = 0;
};


TEST_F(timed_worker_Fixture, Test)
{
    light::timed_worker worker;
    ASSERT_TRUE(worker.initialize());

    boost::asio::io_service service;

    ASSERT_EQ(call_count(), 0);
    worker.add(service, 1000, boost::bind(&timed_worker_Fixture::test, this), "test");

    service.run_one();
    ASSERT_EQ(call_count(), 1);

    worker.release();
}