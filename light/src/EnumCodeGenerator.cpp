#include "light.h"
#include "EnumCodeGenerator.h"
#include "FileUtil.h"

namespace light
{

bool EnumCodeGenerator::FileWrite(const std::wstring& file_name, const std::wstring& str)
{
	std::wofstream fileStream;
	fileStream.open(file_name.c_str(), std::ios_base::app);
	if ( true == fileStream.fail())
		return false;

	/// 빈 파일이문~
	std::streamoff size = fileStream.rdbuf()->pubseekoff(0, std::ios_base::end);;
	if( 0 == size )
	{
		fileStream << L"#pragma once" << std::endl;
		fileStream << std::endl;
	}

	fileStream << str;
	fileStream.close();
	return true;
}

bool EnumCodeGenerator::Open(const std::wstring& name, const std::wstring& ClassName, const std::wstring& NameSpace)
{
	m_ClassName = ClassName;
	m_FileName = name; 

	m_NameSpace = NameSpace;

	if ( false == m_NameSpace.empty() )
	{
		NameSpaceBegin(m_Stream, m_NameSpace);
		EndLine(m_Stream);
	}

	/// 클래스 선언 시작
	ClassBegin(m_Stream, ClassName);
	return true;
}

bool EnumCodeGenerator::End()
{
	if( false == Write() )
	{
		LOG_ERROR(L"Write() Failed.");
		return false;
	}

	if ( false == m_NameSpace.empty() )
	{
		EndLine(m_Stream);
		NameSpaceEnd(m_Stream, m_NameSpace);
	}
	return true;
}

bool EnumCodeGenerator::Write()
{
	/// 메소드 선언 및 구현부
	Public(m_Stream);

	m_Stream << L"\t" << L"enum Type" << std::endl;
	m_Stream << L"\t" << L"{" << std::endl;

	for each(auto& Info in m_Info)
	{
		m_Stream << L"\t" << L"\t" << Info.NAME << L" = " << Info.VALUE << L"," << L" // " << Info.COMMENT << std::endl;
	}

	m_Stream << L"\t" << L"};" << std::endl;

	/// 클래스 선언 종료
	ClassEnd(m_Stream);

	m_Stream << std::endl;
	m_Stream << L"static const std::wstring " << m_ClassName << L"_STR[] = {" << std::endl;

	for each(auto& Info in m_Info)
	{
		m_Stream << L"\t" << L"\t" << WrapQuot(Info.COMMENT) << L", " << std::endl;
	}

	m_Stream << L"};";
	m_Stream << std::endl;
	return true;
}

bool EnumCodeGenerator::Info(const std::wstring& name, const INT value, const std::wstring& comment)
{
	m_Info.push_back(INFO(name, value, comment));
	return true;
}

} //namespace light
