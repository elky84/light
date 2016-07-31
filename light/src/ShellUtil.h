#pragma once

class ShellUtil
{
public:
	static int execute(const std::wstring& command, OUT std::wstring& strStream);
};