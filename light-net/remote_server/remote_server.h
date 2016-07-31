#pragma once

#include "EnvironmentManager.h"
#include "Singleton.h"
#include "PipeServer.h"
#include "ProcessUtil.h"
#include <memory>
#include "GuiPrompt.h"

class remote_server : public light::EnvironmentManager
{
public:
	remote_server(const std::wstring& program_name, const std::wstring& arg);
	~remote_server();

	virtual bool initialize();

	virtual void on_update();
	
	virtual void release();

private:
	std::shared_ptr<light::net::PipeServer> m_Server;
};
