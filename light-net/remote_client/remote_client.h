#pragma once

#include "EnvironmentManager.h"
#include "PipeClient.h"

class remote_client : public light::EnvironmentManager
{
public:
	remote_client(const std::wstring& program_name, const std::wstring& arg);
	~remote_client();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	std::unique_ptr<light::net::PipeClient> m_Pipe;
};
