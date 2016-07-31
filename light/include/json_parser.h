#pragma once

#include <boost/property_tree/ptree.hpp> 

namespace light
{

namespace json
{

class parser
{
public:
    parser(void);
    ~parser(void);

    bool load(const std::wstring& file_name);

    void set(const std::string& str);

    template <typename T>
    T get(const std::string& key)
    {
        if (m_load == false)
            throw "not loaded data. " + key;

        try
        {
            return m_tree.get<T>(key);
        }
        catch (std::exception const& e)
        {
            LOG_ERROR(L"%S", e.what());
        }
        throw "not find node. " + key;
    }

    template <typename T>
    T get_child(const std::string& key)
    {
        if (m_load == false)
            throw "not loaded data. " + key;

        try
        {
            return m_tree.get_child<T>(key);
        }
        catch (std::exception const& e)
        {
            LOG_ERROR(L"%S", e.what());
        }
        throw "not find child. " + key;
    }

    boost::property_tree::ptree& get_child(const std::string& key)
    {
        if (m_load == false)
            throw "not loaded data. " + key;

        try
        {
            return m_tree.get_child(key);
        }
        catch (std::exception const& e)
        {
            LOG_ERROR(L"%S", e.what());
        }
        throw "not find child. " + key;
    }

    void print(boost::property_tree::ptree const& pt)
    {
        using boost::property_tree::ptree;
        ptree::const_iterator end = pt.end();
        for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
            std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
            print(it->second);
        }
    }

    boost::property_tree::ptree& tree()
    {
        return m_tree;
    }

private:
    boost::property_tree::ptree m_tree;

    bool m_load;
};

} // namespace json

} //namespace light
