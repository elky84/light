#pragma once

#include <string>
#include <vector>
#include <map>

namespace light
{

class CommandLineArguments
{
public:
    static bool has(const std::wstring& key);
    static std::wstring& to_str(const std::wstring& key);
    static int to_int(const std::wstring& key);

public:
    static bool parse(const std::wstring& args);

public:
    template <typename T>
    static void has_if_assign(const std::wstring&key, T func)
    {
        if(has(key))
            func(to_str(key));
    }

private:
	static std::map<std::wstring, std::wstring> m_tokens;
};

} // namespace light