#include "light.h"
#include "LogHelper.h"

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/log/utility/setup/from_stream.hpp>

#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include <boost/log/sinks/text_multifile_backend.hpp>

#include <boost/core/null_deleter.hpp>

#include <fstream>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

//[ example_extension_system_uptime_attr_impl
// The function returns the system uptime, in seconds
unsigned int get_uptime()
{
#if defined(BOOST_WINDOWS)
    return GetTickCount() / 1000u;
#elif defined(__linux__) || defined(__linux) || defined(linux)
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        BOOST_THROW_EXCEPTION(std::runtime_error("Could not acquire uptime"));
    return info.uptime;
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    struct timeval boottime;
    std::size_t len = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    if (sysctl(mib, 2, &boottime, &len, NULL, 0) < 0)
        BOOST_THROW_EXCEPTION(std::runtime_error("Could not acquire uptime"));
    return time(NULL) - boottime.tv_sec;
#elif (defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)) && defined(CLOCK_UPTIME)
    struct timespec ts;
    if (clock_gettime(CLOCK_UPTIME, &ts) != 0)
        BOOST_THROW_EXCEPTION(std::runtime_error("Could not acquire uptime"));
    return ts.tv_sec;
#else
    return 0;
#endif
}

// attribute implementation class
class system_uptime_impl :
    public logging::attribute::impl
{
public:
    // The method generates a new attribute value
    logging::attribute_value get_value()
    {
        return attrs::make_attribute_value(get_uptime());
    }
};
//]

//[ example_extension_system_uptime_attr
// attribute interface class
class system_uptime :
    public logging::attribute
{
public:
    system_uptime() : logging::attribute(new system_uptime_impl())
    {
    }
    // attribute casting support
    explicit system_uptime(attrs::cast_source const& source) : logging::attribute(source.as< system_uptime_impl >())
    {
    }
};

typedef src::severity_channel_logger_mt<
    boost::log::trivial::severity_level,     // the type of the severity level
    std::string         // the type of the channel name
> main_logger_mt;

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(main_logger, main_logger_mt)
{
    main_logger_mt lg(keywords::channel = "main");
    return lg;
}

//typedef src::severity_channel_logger_mt<
//    boost::log::trivial::severity_level,     // the type of the severity level
//    std::string         // the type of the channel name
//> app_logger_mt;
//
//BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(app_logger, app_logger_mt)
//{
//    app_logger_mt lg(keywords::channel = "app");
//    return lg;
//}
//
//typedef src::severity_channel_logger_mt<
//    boost::log::trivial::severity_level,     // the type of the severity level
//    std::string         // the type of the channel name
//> stat_logger_mt;
//
//BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(stat_logger, stat_logger_mt)
//{
//    stat_logger_mt lg(keywords::channel = "stat");
//    return lg;
//}

namespace light
{

LogHelper::LogHelper()
{
}

LogHelper::~LogHelper()
{

}

bool LogHelper::initialize()
{
    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("Scope", attrs::named_scope());
    logging::core::get()->add_global_attribute("SystemUptime", system_uptime());
    logging::core::get()->add_global_attribute("Uptime", attrs::timer());
    logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());

    logging::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    logging::register_simple_filter_factory<boost::log::trivial::severity_level, char>("Severity");

    // configuration file setting : http://boost-log.sourceforge.net/libs/log/doc/html/log/detailed/utilities.html#log.detailed.utilities.setup.settings
    std::ifstream file("log_setting.ini");
    if (file.is_open())
    {
        logging::init_from_stream(file);
    }
    else
    {
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] <%Severity%> [%Scope%] %Message%", boost::log::keywords::auto_flush = true);

        // default severity is over info.
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
        boost::shared_ptr< sinks::text_multifile_backend > backend = boost::make_shared< sinks::text_multifile_backend >();
        backend->set_file_name_composer(sinks::file::as_file_name_composer(expr::stream << "log/" << expr::attr< std::string >("Channel") << ".log"));

        typedef sinks::synchronous_sink< sinks::text_multifile_backend > sink_t;
        boost::shared_ptr< sink_t > sink(new sink_t(backend));

        sink->set_formatter(expr::format("[%1%] [%2%] [%3%] <%4%> %5% %6%")
            % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
            % expr::attr< unsigned int >("SystemUptime")
            % expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
            % expr::attr< boost::log::trivial::severity_level >("Severity")
            % expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
            % expr::message);

        logging::core::get()->add_sink(sink);
    }
    return true;
}

bool LogHelper::release()
{
    logging::core::get()->flush();
    logging::core::get()->remove_all_sinks();
    return true;
}

void LogHelper::flush()
{
    logging::core::get()->flush();
}


void LogHelper::log(boost::log::trivial::severity_level level, boost::wformat& message)
{
    BOOST_LOG_SEV(main_logger::get(), level) << message.str();
}

} // namespace light