#include "light.h"
#include "ShellPrompt.h"

#include <conio.h>

namespace light
{

ShellPrompt::ShellPrompt(const end_callback end, const std::wstring& notice)
: PromptInterface(end, notice)
{
}

ShellPrompt::~ShellPrompt()
{
}

void ShellPrompt::begin()
{
    std::wcout << L">> ";
}

void ShellPrompt::on_update()
{
	if( 0 != _kbhit() )
	{
		std::getline(std::wcin, m_command);
		call();
	}
}

void ShellPrompt::notice(const std::wstring& notice)
{
	__super::notice(notice);
	std::wcout << m_notice << std::endl;
}


bool ShellPrompt::call()
{
	bool ret = __super::call();
	std::wcout << L">> ";
	return ret;
}


} //namespace light
