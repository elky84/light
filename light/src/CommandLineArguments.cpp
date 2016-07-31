#include "light.h"
#include "CommandLineArguments.h"
#include "Str.h"
#include <cstring>
#include <cctype>

namespace light
{

std::map<std::wstring, std::wstring> CommandLineArguments::m_tokens;

bool CommandLineArguments::has(const std::wstring &key)
{
	if( m_tokens.find(str::to_lower(key)) == m_tokens.end() )
	{
		LOG_DEBUG(L"find failed. key[%s]", key.c_str());
		return false;
	}
	return true;
}

std::wstring& CommandLineArguments::to_str(const std::wstring &key)
{
	return m_tokens[str::to_lower(key)];
}

int CommandLineArguments::to_int(const std::wstring &key)
{
    return _wtoi(m_tokens[str::to_lower(key)].c_str());
}

bool CommandLineArguments::parse(const std::wstring& args)
{
    std::vector<std::wstring> tokens = str::split(args, L"-");
    if (tokens.empty())
        return false;

	std::for_each(tokens.begin(), tokens.end(), [](const std::wstring& value) {
        auto vec = str::split(value, L" ");
        if (1 == vec.size())
        {
            m_tokens[str::to_lower(vec[0])] = L"";
        }
        else if( 2 <= vec.size())
        {
            m_tokens[str::to_lower(vec[0])] = vec[1];
        }
	});

    return true;
}
} // namespace light