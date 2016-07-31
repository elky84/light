#include "light-net.h"

namespace light
{
namespace net
{

std::string get_external_ip(boost::asio::io_service& io_service)
{
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);

    while (it != boost::asio::ip::tcp::resolver::iterator())
    {
        boost::asio::ip::address addr = (it++)->endpoint().address();
        if (addr.is_v4())
        {
            return addr.to_string();
        }
    }
    return "";
}

} // namespace net

} // namespace light
