#pragma once

namespace Instance
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


} // Instance

