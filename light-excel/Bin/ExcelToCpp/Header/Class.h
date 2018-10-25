#pragma once

namespace Instance
{

class Class
{
public:
	Class()
	{
		memset(&m_Type, 0, sizeof(m_Type));

		memset(&m_Index, 0, sizeof(m_Index));

	}

	int& TypeReference()
	{
		return m_Type;
	}

	int Type() const
	{
		return m_Type;
	}

	void Type(const int& _Type)
	{
		m_Type = _Type;
	}

	std::wstring& NameReference()
	{
		return m_Name;
	}

	std::wstring Name() const
	{
		return m_Name;
	}

	void Name(const std::wstring& _Name)
	{
		m_Name = _Name;
	}

	int& IndexReference()
	{
		return m_Index;
	}

	int Index() const
	{
		return m_Index;
	}

	void Index(const int& _Index)
	{
		m_Index = _Index;
	}

private:
	int m_Type;
	std::wstring m_Name;
	int m_Index;
};


} // Instance

