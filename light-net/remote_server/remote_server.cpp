#include "stdafx.h"
#include "remote_server.h"

#include "PipeServer.h"
#include "client_peer.h"

#include "DirectoryUtil.h"
#include "FileUtil.h"
#include "CommandLineArguments.h"

#include <boost/bind.hpp>

remote_server::remote_server(const std::wstring& program_name, const std::wstring& arg)
	: light::EnvironmentManager(program_name, arg)
{
}

remote_server::~remote_server()
{
}

bool remote_server::initialize()
{
	if( false == light::CommandLineArguments::has(L"PIPENAME") )
	{
		LOG_ERROR(L"This program need PIPENAME Parameter.");
		return false;
	}

	std::wstring strPipeName = L"\\\\.\\pipe\\";
	strPipeName.append(light::CommandLineArguments::to_str(L"PIPENAME"));

	light::net::PipeServer* pipeserver = new light::net::PipeServer(strPipeName, new client_peer);
	if(false == pipeserver->listen())
	{
		stop();
		return false;
	}

	m_Server.reset(pipeserver);
	m_Server->start();
	on_complete();
	return true;
}

void remote_server::on_update()
{
	Sleep(100);

	m_Server->on_update();
}

void remote_server::release()
{
	m_Server->release();
	light::net::Pipe::release();

}
