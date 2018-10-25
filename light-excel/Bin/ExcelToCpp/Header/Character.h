#pragma once

namespace Instance
{

class Character
{
public:
	Character()
	{
		m_Decide_.resize(6);

		memset(&m_Wis, 0, sizeof(m_Wis));

		memset(&m_Str, 0, sizeof(m_Str));

		memset(&m_Range, 0, sizeof(m_Range));

		memset(&m_Phy, 0, sizeof(m_Phy));

		memset(&m_Move, 0, sizeof(m_Move));

		memset(&m_Luck, 0, sizeof(m_Luck));

		memset(&m_Job, 0, sizeof(m_Job));

		memset(&m_Int, 0, sizeof(m_Int));

		memset(&m_Index, 0, sizeof(m_Index));

		memset(&m_Dex, 0, sizeof(m_Dex));

		memset(&m_Decide_Cooltime, 0, sizeof(m_Decide_Cooltime));

		memset(&m_Cost, 0, sizeof(m_Cost));

		memset(&m_Class, 0, sizeof(m_Class));

		memset(&m_Att_Speed, 0, sizeof(m_Att_Speed));

		memset(&m_Att_Cooltime_Type, 0, sizeof(m_Att_Cooltime_Type));

		memset(&m_Att_Cooltime, 0, sizeof(m_Att_Cooltime));

		memset(&m_Agi, 0, sizeof(m_Agi));

	}

	int& WisReference()
	{
		return m_Wis;
	}

	int Wis() const
	{
		return m_Wis;
	}

	void Wis(const int& _Wis)
	{
		m_Wis = _Wis;
	}

	int& StrReference()
	{
		return m_Str;
	}

	int Str() const
	{
		return m_Str;
	}

	void Str(const int& _Str)
	{
		m_Str = _Str;
	}

	int& RangeReference()
	{
		return m_Range;
	}

	int Range() const
	{
		return m_Range;
	}

	void Range(const int& _Range)
	{
		m_Range = _Range;
	}

	int& PhyReference()
	{
		return m_Phy;
	}

	int Phy() const
	{
		return m_Phy;
	}

	void Phy(const int& _Phy)
	{
		m_Phy = _Phy;
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

	int& MoveReference()
	{
		return m_Move;
	}

	int Move() const
	{
		return m_Move;
	}

	void Move(const int& _Move)
	{
		m_Move = _Move;
	}

	int& LuckReference()
	{
		return m_Luck;
	}

	int Luck() const
	{
		return m_Luck;
	}

	void Luck(const int& _Luck)
	{
		m_Luck = _Luck;
	}

	int& JobReference()
	{
		return m_Job;
	}

	int Job() const
	{
		return m_Job;
	}

	void Job(const int& _Job)
	{
		m_Job = _Job;
	}

	int& IntReference()
	{
		return m_Int;
	}

	int Int() const
	{
		return m_Int;
	}

	void Int(const int& _Int)
	{
		m_Int = _Int;
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

	std::wstring& ImageReference()
	{
		return m_Image;
	}

	std::wstring Image() const
	{
		return m_Image;
	}

	void Image(const std::wstring& _Image)
	{
		m_Image = _Image;
	}

	int& DexReference()
	{
		return m_Dex;
	}

	int Dex() const
	{
		return m_Dex;
	}

	void Dex(const int& _Dex)
	{
		m_Dex = _Dex;
	}

	int& Decide_CooltimeReference()
	{
		return m_Decide_Cooltime;
	}

	int Decide_Cooltime() const
	{
		return m_Decide_Cooltime;
	}

	void Decide_Cooltime(const int& _Decide_Cooltime)
	{
		m_Decide_Cooltime = _Decide_Cooltime;
	}

	int& Decide_5Reference()
	{
		return m_Decide_[5];
	}

	std::vector<int>& VectorDecide_()
	{
		return m_Decide_;
	}

	int& Decide_4Reference()
	{
		return m_Decide_[4];
	}

	int& Decide_3Reference()
	{
		return m_Decide_[3];
	}

	int& Decide_2Reference()
	{
		return m_Decide_[2];
	}

	int& Decide_1Reference()
	{
		return m_Decide_[1];
	}

	int& CostReference()
	{
		return m_Cost;
	}

	int Cost() const
	{
		return m_Cost;
	}

	void Cost(const int& _Cost)
	{
		m_Cost = _Cost;
	}

	int& ClassReference()
	{
		return m_Class;
	}

	int Class() const
	{
		return m_Class;
	}

	void Class(const int& _Class)
	{
		m_Class = _Class;
	}

	int& Att_SpeedReference()
	{
		return m_Att_Speed;
	}

	int Att_Speed() const
	{
		return m_Att_Speed;
	}

	void Att_Speed(const int& _Att_Speed)
	{
		m_Att_Speed = _Att_Speed;
	}

	int& Att_Cooltime_TypeReference()
	{
		return m_Att_Cooltime_Type;
	}

	int Att_Cooltime_Type() const
	{
		return m_Att_Cooltime_Type;
	}

	void Att_Cooltime_Type(const int& _Att_Cooltime_Type)
	{
		m_Att_Cooltime_Type = _Att_Cooltime_Type;
	}

	int& Att_CooltimeReference()
	{
		return m_Att_Cooltime;
	}

	int Att_Cooltime() const
	{
		return m_Att_Cooltime;
	}

	void Att_Cooltime(const int& _Att_Cooltime)
	{
		m_Att_Cooltime = _Att_Cooltime;
	}

	int& AgiReference()
	{
		return m_Agi;
	}

	int Agi() const
	{
		return m_Agi;
	}

	void Agi(const int& _Agi)
	{
		m_Agi = _Agi;
	}

private:
	int m_Wis;
	int m_Str;
	int m_Range;
	int m_Phy;
	std::wstring m_Name;
	int m_Move;
	int m_Luck;
	int m_Job;
	int m_Int;
	int m_Index;
	std::wstring m_Image;
	int m_Dex;
	int m_Decide_Cooltime;
	std::vector<int> m_Decide_;
	int m_Cost;
	int m_Class;
	int m_Att_Speed;
	int m_Att_Cooltime_Type;
	int m_Att_Cooltime;
	int m_Agi;
};


} // Instance

