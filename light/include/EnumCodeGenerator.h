#pragma once

#include "CodeGenerateUtil.h"

namespace light
{

class EnumCodeGenerator 
	: public CodeGenrateUtil
{
protected:
	struct INFO
	{
	public:
		std::wstring NAME;

		INT VALUE;

		std::wstring COMMENT;

	public:
		/**
		@brief 생성자.
		*/
		INFO()
			: VALUE(0)
		{
		}

		/**
		@brief 생성자. 
		@param name 이름
		@param value 값
		@param comment 이름
		*/
		INFO(const std::wstring& name, const INT value, const std::wstring& comment)
			: NAME(name)
			, VALUE(value)
			, COMMENT(comment)
		{
		}
	};

protected:
	typedef std::vector<INFO> VECTOR_INFO;

public:
	virtual bool Open(const std::wstring& name, const std::wstring& ClassName, const std::wstring& NameSpace);

	bool End();

	virtual bool Info(const std::wstring& Name, const INT value, const std::wstring& comment);

	std::wstring Doc()
	{
		return m_Stream.str();
	}

	bool FileWrite(const std::wstring& file_name, const std::wstring& str);

protected:
	virtual bool Write();

protected:
	VECTOR_INFO m_Info;

	std::wstring m_ClassName;

	std::wstring m_EnumName;

	std::wstring m_FileName;

	std::wstring m_NameSpace;

protected:
	std::wstringstream m_Stream;
};

} //namespace light
