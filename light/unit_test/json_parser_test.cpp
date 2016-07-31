#include "stdafx.h"
#include "json_parser.h"

#include <boost/foreach.hpp>

TEST(json_parser, read_file)
{
    light::json::parser parser;
    ASSERT_TRUE(parser.load(L"test.json"));

    using boost::property_tree::ptree;

    std::string encoding = parser.get<std::string>("encoding");
    std::cout << "Encoding=" << encoding << std::endl;

    std::string config_type = parser.get<std::string>("config_type");
    std::cout << "config_type=" << config_type << std::endl;

    unsigned short port = parser.get<unsigned short>("port");
    std::cout << "port=" << port << std::endl;

    std::cout << "[Log]" << std::endl;
    ptree& log_child = parser.get_child("log");
    int size = log_child.get<int>("max_size");
    std::cout << "size=" << size << std::endl;

    bool write_file = log_child.get<bool>("write_file");
    std::cout << "write_file=" << std::boolalpha << write_file << std::endl;

    std::cout.setf(std::ios::boolalpha);
    bool print_screen = log_child.get<bool>("print_screen");
    std::cout << "print_screen=" << print_screen << std::endl;

    std::cout << "[Database]" << std::endl;
    ptree &db_child = parser.get_child("database");

    // array!
    int idx = 0;
    BOOST_FOREACH(ptree::value_type &vt, db_child)
    {
        std::string desc = vt.second.get<std::string>("desc");
        std::string address = vt.second.get<std::string>("address");
        unsigned short db_port = vt.second.get<unsigned short>("port");
        std::string instance = vt.second.get<std::string>("instance");
        std::string id = vt.second.get<std::string>("id");
        std::string password = vt.second.get<std::string>("password");

        std::cout << idx++ << " " << desc << " " << address << " " << db_port << " " << instance << " " << id << " " << password << std::endl;
    }
}