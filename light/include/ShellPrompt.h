#pragma once

#include "PromptInterface.h"

namespace light
{

class ShellPrompt 
    : public PromptInterface
{
public:
	ShellPrompt(const end_callback end, const std::wstring& notice = L"");
	
	virtual ~ShellPrompt();

    virtual void begin();

	virtual void notice(const std::wstring& notice);

	virtual bool call();

private:
	virtual void on_update();
};

} //namespace light
