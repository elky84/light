#pragma once

#include <boost/property_tree/ptree.hpp> 

namespace light
{

namespace INI
{

class Parser
{
public:
	Parser(void);
	~Parser(void);

	bool load(const std::wstring& file_name);

	void set(const std::string& str);

	template <typename T>
	T get(const std::string& key)
	{
		if( m_load == false )
			return T();

		try
		{
			return m_tree.get<T>(key);
		}
		catch(std::exception const& e)
		{
            LOG_ERROR(L"%S", e.what());
        }
		return T();
	}

private:
	boost::property_tree::ptree m_tree;

	bool m_load;
};

} // namespace INI

} //namespace light
