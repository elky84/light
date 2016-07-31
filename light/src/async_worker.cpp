 #include "light.h"
#include "async_worker.h"

namespace light
{

async_worker::async_worker(void)
    : m_io_service_work(m_io_service)
    , m_io_count(0)
{
}

async_worker::~async_worker(void)
{
    destroy();
}

void async_worker::destroy(void)
{
    stop();
}

bool async_worker::start(int io_count, int strand_count)
{
    m_io_service.reset();
    m_io_count = io_count;

    for (int i = 0; i < io_count; i++)
    {
        m_thread_group.create_thread(boost::bind(&async_worker::wrapped_run, this));
    }

    for (int i = 0; i < strand_count; ++i)
    {
        m_strands.push_back(boost::asio::strand(m_io_service));
    }
    return true;
}


void	async_worker::stop(void)
{
    m_thread_group.interrupt_all();
    m_io_service.stop();
    m_thread_group.join_all();
}

void	async_worker::wrapped_run(void)
{
    boost::this_thread::interruption_point();

    m_io_service.run();
}

boost::asio::io_service& async_worker::get_io_service(void)
{
    return m_io_service;
}

void async_worker::increase_thread(int count)
{
    for (int i = 0; i < count; i++)
    {
        m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));

        m_io_count++;
    }
}

void async_worker::run_one(void)
{
    m_io_service.run_one();
}

async_worker_limit::async_worker_limit(void)
    : m_queue_count(0)
    , m_input_count(0)
    , m_output_count(0)
{
}

async_worker_limit::~async_worker_limit(void)
{

}

} // namespace light
