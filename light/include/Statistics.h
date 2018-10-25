#pragma once

#pragma warning ( push )
#pragma warning ( disable:4714 ) 
#include <boost/format.hpp>
#pragma warning ( pop )

#include <boost/atomic.hpp>

namespace light
{

    template <typename T>
    class Statistics
    {
    public:
        typedef double SUM;

        typedef double AVERAGE;

        typedef long COUNT;

    public:

        Statistics()
            : m_sum(0)
            , m_count(0)
        {
        }

        Statistics(const T& data)
            : m_sum(0)
            , m_count(0)
        {
            record(data);
        }

        virtual ~Statistics()
        {
        }

        virtual void record(const T& data)
        {
            m_sum.exchange(m_sum + (SUM)data);
            m_count.fetch_add(1);
        }

        virtual void clear()
        {
            m_sum.exchange(0);
            m_count.exchange(0);
        }

        const AVERAGE average() const
        {
            if (m_count.load() == 0)
                return 0.0f;

            return m_sum.load() / (AVERAGE)m_count.load();
        }

        const SUM sum() const
        {
            return m_sum.load();
        }

        const COUNT count() const
        {
            return m_count.load();
        }

        virtual std::string to_log()
        {
            return boost::str(boost::format("count[%1%] sum[%2%] average[%3%]") % count() % sum() % average());
        }

    protected:
        boost::atomic<COUNT> m_count;

        boost::atomic<SUM> m_sum;
    };

    template <typename T>
    class StatisticsSuccess
        : public Statistics<T>
    {
    public:
        StatisticsSuccess()
            : m_success(0)
        {
        }

        StatisticsSuccess(const T& data)
            : Statistics(data)
            , m_success(0)
        {
            record(data);
        }

        virtual ~StatisticsSuccess()
        {
        }

        virtual void record(const T& data)
        {
            __super::record(data);
            if (data)
            {
                m_success.fetch_add(1);
            }
        }

        virtual void clear()
        {
            __super::clear();
            m_success.exchange(0);
        }

        COUNT success() const
        {
            return m_success.load();
        }

        COUNT fail() const
        {
            return m_count.load() - m_success.load();
        }

        virtual std::string to_log()
        {
            return __super::to_log() + boost::str(boost::format(" success[%1%] fail[%2%]") % success() % fail());
        }

    private:
        boost::atomic<COUNT> m_success;
    };

    template <typename T>
    class StatisticsMinMax
        : public Statistics< T>
    {
    public:
        StatisticsMinMax()
            : m_minimum(0)
            , m_maximum(0)
        {
        }

        StatisticsMinMax(const T& data)
            : m_minimum(0)
            , m_maximum(0)
        {
            record(data);
        }

        virtual ~StatisticsMinMax()
        {
        }

        virtual void record(const T& data)
        {
            if (count() == 0)
            {
                m_minimum.exchange(data);
                m_maximum.exchange(data);
            }
            else
            {
                m_minimum.exchange(std::min(m_minimum.load(), data));
                m_maximum.exchange(std::max(m_maximum.load(), data));
            }
            __super::record(data);
        }

        const T minimum() const
        {
            return m_minimum.load();
        }

        const T maximum() const
        {
            return m_maximum.load();
        }

        virtual void clear()
        {
            m_minimum.exchange(0);
            m_maximum.exchange(0);

            __super::clear();
        }

        virtual std::string to_log()
        {
            return __super::to_log() + boost::str(boost::format(" min[%1%] max[%2%]") % minimum() % maximum());
        }

    private:
        boost::atomic<T> m_minimum;

        boost::atomic<T> m_maximum;
    };


} //namespace light