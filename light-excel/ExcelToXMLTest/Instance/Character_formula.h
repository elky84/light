#pragma once

namespace ExcelData
{

class Character_Formula
{
public:
	Character_Formula()
	{
		memset(&m_TOTAL, 0, sizeof(m_TOTAL));

		memset(&m_Job_Name, 0, sizeof(m_Job_Name));

		memset(&m_Index, 0, sizeof(m_Index));

		memset(&m_HP, 0, sizeof(m_HP));

		memset(&m_HIT, 0, sizeof(m_HIT));

		memset(&m_DODGE, 0, sizeof(m_DODGE));

		memset(&m_CRITICAL, 0, sizeof(m_CRITICAL));

		memset(&m_AP_DMG, 0, sizeof(m_AP_DMG));

		memset(&m_AP_DF, 0, sizeof(m_AP_DF));

		memset(&m_AD_DMG, 0, sizeof(m_AD_DMG));

		memset(&m_AD_DF, 0, sizeof(m_AD_DF));

	}

	float& TOTALReference()
	{
		return m_TOTAL;
	}

	float TOTAL() const
	{
		return m_TOTAL;
	}

	void TOTAL(const float& _TOTAL)
	{
		m_TOTAL = _TOTAL;
	}

	std::wstring& Job_NameReference()
	{
		return m_Job_Name;
	}

	std::wstring Job_Name() const
	{
		return m_Job_Name;
	}

	void Job_Name(const std::wstring& _Job_Name)
	{
		m_Job_Name = _Job_Name;
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

	float& HPReference()
	{
		return m_HP;
	}

	float HP() const
	{
		return m_HP;
	}

	void HP(const float& _HP)
	{
		m_HP = _HP;
	}

	float& HITReference()
	{
		return m_HIT;
	}

	float HIT() const
	{
		return m_HIT;
	}

	void HIT(const float& _HIT)
	{
		m_HIT = _HIT;
	}

	float& DODGEReference()
	{
		return m_DODGE;
	}

	float DODGE() const
	{
		return m_DODGE;
	}

	void DODGE(const float& _DODGE)
	{
		m_DODGE = _DODGE;
	}

	float& CRITICALReference()
	{
		return m_CRITICAL;
	}

	float CRITICAL() const
	{
		return m_CRITICAL;
	}

	void CRITICAL(const float& _CRITICAL)
	{
		m_CRITICAL = _CRITICAL;
	}

	float& AP_DMGReference()
	{
		return m_AP_DMG;
	}

	float AP_DMG() const
	{
		return m_AP_DMG;
	}

	void AP_DMG(const float& _AP_DMG)
	{
		m_AP_DMG = _AP_DMG;
	}

	float& AP_DFReference()
	{
		return m_AP_DF;
	}

	float AP_DF() const
	{
		return m_AP_DF;
	}

	void AP_DF(const float& _AP_DF)
	{
		m_AP_DF = _AP_DF;
	}

	float& AD_DMGReference()
	{
		return m_AD_DMG;
	}

	float AD_DMG() const
	{
		return m_AD_DMG;
	}

	void AD_DMG(const float& _AD_DMG)
	{
		m_AD_DMG = _AD_DMG;
	}

	float& AD_DFReference()
	{
		return m_AD_DF;
	}

	float AD_DF() const
	{
		return m_AD_DF;
	}

	void AD_DF(const float& _AD_DF)
	{
		m_AD_DF = _AD_DF;
	}

private:
	float m_TOTAL;
	std::wstring m_Job_Name;
	int m_Index;
	float m_HP;
	float m_HIT;
	float m_DODGE;
	float m_CRITICAL;
	float m_AP_DMG;
	float m_AP_DF;
	float m_AD_DMG;
	float m_AD_DF;
};

class Character_FormulaData : public light::excel::Data
{
public:
	virtual bool Load()
	{
		Character_Formula Instance;
		light::XMLParser parser;
		light::ScopeProfiler profiler(__FUNCTIONW__, 10, L"./XML/Character_Formula.XML");
		if( false == parser.read_file(L"./XML/Character_Formula.XML"))
		{
			LOG_ERROR(L"%s, Open() Failed. Path(./XML/Character_Formula.XML");
			return false;
		}

		if ( false == parser.execute(L"/Character_FormulaList"))
		{
			LOG_ERROR(L"%s, execute() Failed. /Character_FormulaList");
			return false;
		}

		if ( false == parser.bind_elem(L"Data"))
		{
			LOG_ERROR(L"%s, execute() Failed. Data");
			return false;
		}

		if ( false == parser.bind_attrib(L"AD_DF", Instance.AD_DFReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.AD_DFReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"AD_DMG", Instance.AD_DMGReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.AD_DMGReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"AP_DF", Instance.AP_DFReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.AP_DFReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"AP_DMG", Instance.AP_DMGReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.AP_DMGReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"CRITICAL", Instance.CRITICALReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.CRITICALReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"DODGE", Instance.DODGEReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.DODGEReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"HIT", Instance.HITReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.HITReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"HP", Instance.HPReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.HPReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Index", Instance.IndexReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.IndexReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"Job_Name", Instance.Job_NameReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.Job_NameReference()");
			return false;
		}

		if ( false == parser.bind_attrib(L"TOTAL", Instance.TOTALReference()))
		{
			LOG_ERROR(L"bind_attrib() Failed. Instance.TOTALReference()");
			return false;
		}

		while (parser.next())
		{
			m_Map.insert(std::map<int, Character_Formula>::value_type(Instance.Index(), Instance));
		}

		return true;
	}

	bool Get(int& key, Character_Formula& Instance)
	{
		auto it = m_Map.find(key);
		{
			return false;
		}
		Instance = it->second;
		return true;
	}

	std::map<int, Character_Formula>& Map()
	{
		return m_Map;
	}

private:
	std::map<int, Character_Formula> m_Map;
};

__declspec(selectany) Character_FormulaData Character_FormulaDataInstance;

} // ExcelData

