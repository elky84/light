#include "light.h"
#include "INIParser.h"

#include <boost/property_tree/ini_parser.hpp>

namespace light
{

namespace INI
{

Parser::Parser()
	: m_load(false)
{
}

Parser::~Parser()
{
}

bool Parser::load(const std::wstring& file_name)
{
	std::string str = WCHAR_TO_CHAR(file_name.c_str());
	try
	{
		boost::property_tree::ini_parser::read_ini(str, m_tree);
	}
	catch(boost::property_tree::ini_parser_error& err)
	{
        LOG_ERROR(L"%S", err.message().c_str());
        return false;
	}
	m_load = true;
	return true;
}

void Parser::set(const std::string& str)
{
	std::stringstream stream;
	stream << str;
	try
	{
		boost::property_tree::ini_parser::read_ini(stream, m_tree);
		m_load = true;
	}
	catch(boost::property_tree::ini_parser_error& err)
	{
        LOG_ERROR(L"%S", err.message().c_str());
    }
}

} // namespace INI

} //namespace light
