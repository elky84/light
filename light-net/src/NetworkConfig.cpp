#include "light-net.h"

#include "NetworkConfig.h"

#include "XMLParser.h"

namespace light
{

namespace net
{

bool NetworkConfig::Load()
{
	std::wstring name = L"Network.xml";
	light::XMLParser reader;
	if( false == reader.read_file(name))
		return false;

	if( false == reader.execute(L"/Network") )
		return false;

	if( false == reader.bind_elem(L"THREAD") )
		return false;

	std::wstring thread_count;
	if( false == reader.bind_attrib(L"COUNT", thread_count) )
		return false;

	if( false == reader.next() )
		return false;

	m_thread_count = _wtoi(thread_count.c_str());
	if( false == reader.out() )
		return false;

	if( false == reader.bind_elem(L"SERVER") )
		return false;

	if( false == reader.bind_attrib(L"IP", m_ip) )
		return false;

	if( false == reader.bind_attrib(L"PORT", m_port) )
		return false;

	if( false == reader.next() )
		return false;

	return true;
}

const std::wstring NetworkConfig::IP() const
{
    return m_ip;
}

const PORT NetworkConfig::Port() const
{
    return (PORT)_wtoi(m_port.c_str());
}

const INT NetworkConfig::thread_count() const
{
    return m_thread_count;
}

} // namespace net

} // namespace light