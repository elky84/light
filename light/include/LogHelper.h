#pragma once

#include "str.h"

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

#pragma warning ( push )
#pragma warning ( disable:4714 ) 
#include <boost/format.hpp>
#pragma warning ( pop )

namespace light
{

class LogHelper
{
public:
    LogHelper();

    virtual ~LogHelper();

    static bool initialize();

    static bool release();

    static void flush();

    static void log(boost::log::trivial::severity_level level, boost::wformat& message);
};

} // namespace light

#pragma warning( disable: 4714 )

static void to_log(boost::log::trivial::severity_level level, boost::wformat& message) {
    light::LogHelper::log(level, message);
}

template<typename TValue, typename... TArgs>
void to_log(boost::log::trivial::severity_level level, boost::wformat& message, TValue arg, TArgs... args) {
    message % arg;
    to_log(level, message, args...);
}

template<typename... TArgs>
void to_log(boost::log::trivial::severity_level level, std::wstring fmt, TArgs... args) {
    boost::wformat message(fmt);
    to_log(level, message, args...);
}

#define LOG_TRACE(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::trace, __VA_ARGS__);\
}

#define LOG_DEBUG(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::debug, __VA_ARGS__);\
}

#define LOG_INFO(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::info, __VA_ARGS__);\
}

#define LOG_WARN(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::warning, __VA_ARGS__);\
}

#define LOG_ERROR(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::error, __VA_ARGS__);\
}

#define LOG_FATAL(...)\
{\
BOOST_LOG_NAMED_SCOPE(__FUNCTION__);\
to_log(boost::log::trivial::fatal, __VA_ARGS__);\
}
