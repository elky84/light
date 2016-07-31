#pragma once

#include "CodeGenerateUtil.h"
#include <set>

namespace light
{

class CppCodeGenerator
	: public CodeGenrateUtil
{
protected:
	struct VARIABLE_INFO
	{
	public:
		/// 변수 타입
		std::wstring TYPE;

		/// 변수 이름
		std::wstring NAME;

		DWORD SIZE;

	public:
		/**
		@brief 생성자.
		*/
		VARIABLE_INFO()
			: SIZE(0)
		{
		}

		/**
		@brief 생성자. 
		@param type 타입
		@param name 이름
		@param size 크기
		*/
		VARIABLE_INFO(const std::wstring& type, const std::wstring& name, const DWORD size = 0)
			: TYPE(type)
			, NAME(name)
			, SIZE(size)
		{
		}
	};

protected:
	typedef std::map<std::wstring, VARIABLE_INFO> MAP_VARIABLE;


public:
	virtual bool Open(const std::wstring& name, const std::wstring& ClassName, const std::wstring& NameSpace);

	bool End();

	virtual bool Variable(const std::wstring& Type, const std::wstring& Name, const DWORD size = 0);

	std::wstring Doc()
	{
		return m_Stream.str();
	}

	bool FileWrite(const std::wstring& file_name, const std::wstring& str);

protected:
	virtual bool Write();

protected:
	MAP_VARIABLE m_Variable;

	std::wstring m_ClassName;

	std::wstring m_FileName;

	std::wstring m_NameSpace;

	std::set<std::wstring> m_setUsedVariable;

private:
	std::wstringstream m_StreamMethod;

	std::wstringstream m_StreamVariable;

protected:
	std::wstringstream m_Stream;
};

} //namespace light
