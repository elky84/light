#pragma once

#include "CppCodeGenerator.h"

namespace light
{

namespace excel
{

class ExcelToXMLCodeGenerator 
	: public light::CppCodeGenerator
{
public:
	virtual bool Open(const std::wstring& FileName, const std::wstring& ClassName, const std::wstring& Key);

	virtual bool Variable(const std::wstring& Type, const std::wstring& Name, const DWORD size = 0);

private:
	virtual bool Write();

private:
	std::wstring m_Key;

	std::wstring m_KeyType;

	std::wstring m_ExcelLoader;
};

} // namespace excel

} // namespace light