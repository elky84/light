#include "light.h"
#include "XMLWriter.h"

namespace light
{

XMLWriter::XMLWriter(const std::wstring& encoding)
	: m_encoding(encoding)
{

}

XMLWriter::~XMLWriter()
{
	if( true == m_fstream.is_open() &&
		false == m_fstream.fail())
	{
		out();
		m_fstream << L"<?xml version=\"1.0\" encoding=\"" << m_encoding << L"\"?>\n";
		m_fstream << m_markup.GetDoc();
		m_fstream.close();
	}
}

bool XMLWriter::open(const std::wstring& file_name, const std::wstring& title_name)
{
	m_fstream.open(file_name.c_str());
	if ( m_fstream.fail())
	{
		LOG_ERROR(L"Failed. read_file[%s]", file_name.c_str());
		return false;
	}

	into(title_name);
	return true;
}

bool XMLWriter::into(const std::wstring& name)
{
	if( false == m_markup.AddElem(name) )
	{
		return false;
	}

	return m_markup.IntoElem();
}

bool XMLWriter::into_with_attrib(const std::wstring& name)
{
	if( false == m_markup.OutOfElem() )
		return false;

	if( false == m_markup.AddChildElem(name) )
	{
		return false;
	}

	return m_markup.IntoElem();
}

bool XMLWriter::out()
{
	return m_markup.OutOfElem();
}

bool XMLWriter::elem(const std::wstring& name, const std::wstring& data)
{
	if( false == m_markup.IntoElem() )
		return false;

	if( false == m_markup.AddChildElem(name, data) )
		return false;

	return m_markup.OutOfElem();
}

bool XMLWriter::elem(const std::wstring& name)
{
	return m_markup.AddElem(name);
}

bool XMLWriter::child_elem(const std::wstring& name)
{
	return m_markup.AddChildElem(name);
}

bool XMLWriter::elem_end()
{
	return true;
}

bool XMLWriter::child_elem_end()
{
	return true;
}

} //namespace light
