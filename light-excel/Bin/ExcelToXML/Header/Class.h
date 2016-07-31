#pragma once

namespace ExcelData
{

class Class
{
public:
	Class()
	{
		memset(&m_Type, 0, sizeof(m_Type));

		memset(&m_Name, 0, sizeof(m_Name));

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

class ClassData : public light::excel::Data
{
public:
	virtual bool Load()
	{
		Class Instance;
		light::XMLParser parser;
		light::ScopeProfiler profiler(__FUNCTIONW__, 10, L"./XML/Class.XML");
		if( false == parser.read_file(L"./XML/Class.XML"))
		{
			LOG_ERROR(L"%s, Open() Failed. Path(./XML/Class.XML");
			return false;
		}

		if ( false == parser.execute(L"/ClassList"))
		{
			LOG_ERROR(L"%s, execute() Failed. /ClassList");
			return false;
		}

		if ( false == parser.bind_elem(L"Data"))
		{
			LOG_ERROR(L"%s, execute() Failed. Data");
			return false;
		}

		if ( false == parser.bind_attrib(L"Index", Instance.IndexReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.IndexReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Name", Instance.NameReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.NameReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Type", Instance.TypeReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.TypeReference()");
			return false;
		}

		while (parser.next())
		{
			m_Map.insert(std::map<int, Class>::value_type(Instance.Index(), Instance));
		}

		return true;
	}

	bool Get(int& key, Class& Instance)
	{
		auto it = m_Map.find(key);
		{
			return false;
		}
		Instance = it->second;
		return true;
	}

	std::map<int, Class>& Map()
	{
		return m_Map;
	}

private:
	std::map<int, Class> m_Map;
};

__declspec(selectany) ClassData ClassDataInstance;

} // ExcelData

