#pragma once

#include "EnvironmentManager.h"
#include "PromptInterface.h"

#include <memory>

class prompt_test : public light::EnvironmentManager
{
public:
	prompt_test(const std::wstring& program_name);
	~prompt_test();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	bool GuiCommand(const std::wstring& str);

	bool ShellCommand(const std::wstring& str);

private:
	std::unique_ptr<light::PromptInterface> m_prompt;
};
