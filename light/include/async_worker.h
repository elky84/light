#pragma once
#ifndef		__IOSP_H__
#define		__IOSP_H__

#pragma warning ( push )
#pragma warning ( disable:4100 ) 
#include <boost/thread.hpp>
#pragma warning ( pop )

#include <boost/atomic.hpp>

namespace light
{

// input/output service provider
class async_worker
{
protected:
    static const WORD DEFAULT_IO_THREAD_COUNT = 10;

    static const WORD DEFAULT_STRAND_COUNT = 10;

public:
    async_worker(void);
    virtual ~async_worker(void);

public:
    virtual bool start(int io_count = DEFAULT_IO_THREAD_COUNT, int strand_count = DEFAULT_STRAND_COUNT);

    virtual void stop(void);

    virtual void destroy(void);

    virtual boost::asio::io_service& get_io_service(void);

    // do not serialize
    template <typename func>
    inline void	post_job(func handler)
    {
        m_io_service.post(handler);
    }

    // strand for serialize
    template <typename func>
    inline void	strand_post_job(func handler, int position = -1)
    {
        if (position != -1)
            m_strands[position].post(handler);
        else
            m_strands[rand() % m_strands.size()].post(handler);
    }

    void increase_thread(int count = 1); // increase io thread

    void run_one(void);

protected:
    void wrapped_run(void);

protected:
    int m_io_count;

    boost::asio::io_service m_io_service;

    // for guarantee alway io_service's run.
    boost::asio::io_service::work m_io_service_work;

    boost::thread_group	m_thread_group;

    // strand group. do not run at the same time each strand.
    std::vector<boost::asio::strand> m_strands;
};

// input/output service provider
class async_worker_limit
    : public async_worker
{
private:
    template <typename completion_handler>
    struct wrap_functor
    {
        wrap_functor(completion_handler handler, boost::atomic<int>& queue_count, boost::atomic<int>& output_count)
            : m_handler(handler)
            , m_queue_count(queue_count)
            , m_output_count(output_count)
        {

        }

        void on_update()
        {
            m_handler();

            m_output_count.fetch_add(1);
            m_queue_count.fetch_sub(1);
        }

    public:
        completion_handler m_handler;
        boost::atomic<int>& m_queue_count;
        boost::atomic<int>& m_output_count;
    };


public:
    async_worker_limit(void);
    virtual ~async_worker_limit(void);

    template <typename completion_handler>
    inline void	post_job(completion_handler handler)
    {
        m_input_count.fetch_add(1);
        m_queue_count.fetch_add(1);

        wrap_functor<completion_handler> functor(handler, m_queue_count, m_output_count);
        m_io_service.post(boost::bind(&wrap_functor<completion_handler>::on_update, functor));
    }

    ///< 직렬화 되어야 할 완료 핸들러는 이곳으로..
    template <typename completion_handler>
    inline void	strand_post_job(completion_handler handler)
    {
        m_input_count.fetch_add(1);
        m_queue_count.fetch_add(1);

        wrap_functor<completion_handler> functor(handler, m_queue_count, m_output_count);
        m_strand.post(boost::bind(&wrap_functor<completion_handler>::on_update, functor));
    }

    int queue_count() const
    {
        return m_queue_count;
    }

    int input_count() const
    {
        return m_input_count;
    }

    int output_count() const
    {
        return m_output_count;
    }

    void clear()
    {
        m_input_count.exchange(0);
        m_output_count.exchange(0);
    }

protected:
    boost::atomic<int> m_queue_count;

    boost::atomic<int> m_input_count;

    boost::atomic<int> m_output_count;
};

} // namespace light

#endif		// __IOSP_H__