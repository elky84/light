#include "stdafx.h"
#include "async_worker.h"

TEST(async_worker, counting_test)
{
    light::async_worker_limit async_worker;
    async_worker.post_job([] {});

    ASSERT_EQ(async_worker.input_count(), 1);

    async_worker.run_one();

    ASSERT_EQ(async_worker.output_count(), 1);
    ASSERT_EQ(async_worker.output_count(), async_worker.input_count());
}

TEST(async_worker, test)
{
    light::async_worker async_worker;
    int count = 0;
    async_worker.post_job([&]() { count += 1; });

    ASSERT_EQ(count, 0);

    async_worker.run_one();

    ASSERT_EQ(count, 1);
}
