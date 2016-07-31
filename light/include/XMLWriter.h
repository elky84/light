#pragma once

#include "Markup.h"
#include "FileUtil.h"

namespace light
{

class XMLWriter
{
public:
	XMLWriter(const std::wstring& encoding = L"euc-kr");

	~XMLWriter();

	bool open(const std::wstring& file_name, const std::wstring& title_name);

	bool into(const std::wstring& name);

	bool into_with_attrib(const std::wstring& name);

	bool out();

	bool elem(const std::wstring& name, const std::wstring& data);

	bool elem(const std::wstring& name);

	bool elem_end();

	std::wstring read_doc() const
	{
		return m_markup.GetDoc();
	}

	template <typename T>
	bool attribute(const std::wstring& name, const T& Data)
	{
		return m_markup.AddAttrib(name.c_str(), Data);
	}

	bool child_elem(const std::wstring& name);

	bool child_elem_end();

	template <typename T>
	bool child_attribute(const std::wstring& name, const T& Data)
	{
		return m_markup.AddChildAttrib(name.c_str(), Data);
	}

private:
	std::wstring m_encoding;

	std::wofstream m_fstream;

	CMarkup m_markup;
};

} //namespace light
