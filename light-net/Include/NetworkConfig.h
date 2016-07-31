#pragma once

namespace light
{

namespace net
{

class NetworkConfig
{
public:
	bool Load();

    const std::wstring IP() const;

    const PORT Port() const;

    const INT thread_count() const;

private:
	std::wstring m_ip;
	std::wstring m_port;

	INT m_thread_count;
};

} // namespace net

} // namespace light