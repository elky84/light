#include "light.h"
#include "json_parser.h"

#pragma warning ( push )
#pragma warning ( disable:4715 ) 
#include <boost/property_tree/json_parser.hpp>
#pragma warning ( pop )

namespace light
{

namespace json
{

parser::parser()
    : m_load(false)
{
}

parser::~parser()
{
}

bool parser::load(const std::wstring& file_name)
{
    std::string str = WCHAR_TO_CHAR(file_name.c_str());
    try
    {
        boost::property_tree::read_json(str, m_tree);
    }
    catch (boost::property_tree::json_parser_error& err)
    {
        LOG_ERROR(L"%S", err.message().c_str());
        return false;
    }

    m_load = true;
    return true;
}

void parser::set(const std::string& str)
{
    std::stringstream stream;
    stream << str;

    try
    {
        boost::property_tree::read_json(stream, m_tree);
        m_load = true;
    }
    catch (boost::property_tree::json_parser_error& err)
    {
        LOG_ERROR(L"%S", err.message().c_str());

    }
}

} // namespace json

} //namespace light
