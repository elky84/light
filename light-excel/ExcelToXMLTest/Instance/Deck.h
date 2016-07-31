#pragma once

namespace ExcelData
{

class Deck
{
public:
	Deck()
	{
		m_Card.resize(10);

		m_Card1.resize(5);

		memset(&m_Name, 0, sizeof(m_Name));

		memset(&m_Index, 0, sizeof(m_Index));

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

	int& Card9Reference()
	{
		return m_Card[9];
	}

	std::vector<int>& VectorCard()
	{
		return m_Card;
	}

	int& Card8Reference()
	{
		return m_Card[8];
	}

	int& Card7Reference()
	{
		return m_Card[7];
	}

	int& Card6Reference()
	{
		return m_Card[6];
	}

	int& Card5Reference()
	{
		return m_Card[5];
	}

	int& Card4Reference()
	{
		return m_Card[4];
	}

	int& Card3Reference()
	{
		return m_Card[3];
	}

	int& Card2Reference()
	{
		return m_Card[2];
	}

	int& Card14Reference()
	{
		return m_Card1[4];
	}

	std::vector<int>& VectorCard1()
	{
		return m_Card1;
	}

	int& Card13Reference()
	{
		return m_Card1[3];
	}

	int& Card12Reference()
	{
		return m_Card1[2];
	}

	int& Card11Reference()
	{
		return m_Card1[1];
	}

	int& Card10Reference()
	{
		return m_Card1[0];
	}

	int& Card1Reference()
	{
		return m_Card[1];
	}

	int& Card0Reference()
	{
		return m_Card[0];
	}

private:
	std::wstring m_Name;
	int m_Index;
	std::vector<int> m_Card;
	std::vector<int> m_Card1;
};

class DeckData : public light::excel::Data
{
public:
	virtual bool Load()
	{
		Deck Instance;
		light::XMLParser parser;
		light::ScopeProfiler profiler(__FUNCTIONW__, 10, L"./XML/Deck.XML");
		if( false == parser.read_file(L"./XML/Deck.XML"))
		{
			LOG_ERROR(L"%s, Open() Failed. Path(./XML/Deck.XML");
			return false;
		}

		if ( false == parser.execute(L"/DeckList"))
		{
			LOG_ERROR(L"%s, execute() Failed. /DeckList");
			return false;
		}

		if ( false == parser.bind_elem(L"Data"))
		{
			LOG_ERROR(L"%s, execute() Failed. Data");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card0", Instance.Card0Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card0Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card1", Instance.Card1Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card1Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card10", Instance.Card10Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card10Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card11", Instance.Card11Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card11Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card12", Instance.Card12Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card12Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card13", Instance.Card13Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card13Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card14", Instance.Card14Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card14Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card2", Instance.Card2Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card2Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card3", Instance.Card3Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card3Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card4", Instance.Card4Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card4Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card5", Instance.Card5Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card5Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card6", Instance.Card6Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card6Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card7", Instance.Card7Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card7Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card8", Instance.Card8Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card8Reference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Card9", Instance.Card9Reference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Card9Reference()");
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

		while (parser.next())
		{
			m_Map.insert(std::map<int, Deck>::value_type(Instance.Index(), Instance));
		}

		return true;
	}

	bool Get(int& key, Deck& Instance)
	{
		auto it = m_Map.find(key);
		{
			return false;
		}
		Instance = it->second;
		return true;
	}

	std::map<int, Deck>& Map()
	{
		return m_Map;
	}

private:
	std::map<int, Deck> m_Map;
};

__declspec(selectany) DeckData DeckDataInstance;

} // ExcelData

