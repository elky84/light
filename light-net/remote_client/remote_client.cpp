#include "stdafx.h"
#include "remote_client.h"

#include "PipeClient.h"
#include "CommandLineArguments.h"

#include "server_peer.h"

remote_client::remote_client(const std::wstring& program_name, const std::wstring& arg)
	: light::EnvironmentManager(program_name, arg)
{
}

remote_client::~remote_client()
{
}

bool remote_client::initialize()
{
	if( false == light::CommandLineArguments::has(L"PIPENAME" ))
	{
		LOG_ERROR(L"This program need PIPENAME Parameter.");
		return false;
	}

	std::wstring strPipeName = L"\\\\.\\pipe\\";
	strPipeName.append(light::CommandLineArguments::to_str(L"PIPENAME"));

	m_Pipe.reset(new light::net::PipeClient(strPipeName, new server_peer));
	m_Pipe->start();
	m_Pipe->connect();
	on_complete();
	return true;
}

void remote_client::on_update()
{
	Sleep(100);

	m_Pipe->on_update();
}

void remote_client::release()
{
	m_Pipe->release();
}
