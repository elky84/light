#include "stdafx.h"
#include "prompt_test.h"

#include "ShellPrompt.h"
#include "GuiPrompt.h"

#include <boost/bind.hpp>

prompt_test::prompt_test(const std::wstring& program_name)
: light::EnvironmentManager(program_name)
{
}

prompt_test::~prompt_test()
{
}

bool prompt_test::initialize()
{
	LOG_INFO(L"You are choice prompt mode.");
	LOG_INFO(L"1. Gui Prompt");
	LOG_INFO(L"2. Shell Prompt");

	{
		int ch = getchar() - '0';
		switch(ch)
		{
		case 1:
			m_prompt.reset(new light::GuiPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance() )));
			m_prompt->callback(boost::bind(&prompt_test::GuiCommand, this, _1) );
			break;
		case 2:
			m_prompt.reset(new light::ShellPrompt( boost::bind(&EnvironmentManager::stop, EnvironmentManager::Instance() )));
			m_prompt->callback(boost::bind(&prompt_test::ShellCommand, this, _1) );
			break;
		default:
			LOG_ERROR(L"%s Invalid choice. [%d]", __FUNCTIONW__, ch);
			stop();
			return false;
		}

        m_prompt->start();
		m_prompt->notice(L"Wait for the your command.");
	}

    on_complete();
	return true;
}

void prompt_test::on_update()
{
	Sleep(1000);
}

void prompt_test::release()
{
}

bool prompt_test::GuiCommand(const std::wstring& str)
{
	if(str == L"quit")
	{
		stop();
	}
	else
	{
		LOG_INFO(L"Typed string : %s", str.c_str());
		return false;
	}
	return true;
}

bool prompt_test::ShellCommand(const std::wstring& str)
{
	if(str.empty())
		return false;

	if(str == L"quit")
	{
		stop();
	}
	else
	{
		std::wcout << L"Input string is " << str << std::endl;
		return false;
	}
	return true;
}
