#pragma once

#include <Windows.h>
#include <atomic>

namespace light
{

class Tick
{
public:
	Tick(const DWORD dwValidTime = DEFAULT_VALID_TIME) 
		: m_ValidTime(dwValidTime)
		, m_LastTime(GetTickCount())
	{
	}

	~Tick()
	{
	}
		
	bool Check(const DWORD current) const
	{
		if (m_ValidTime <= delta<DWORD>(m_LastTime.load(), current))	//m_ValidTime만큼의 시간동안 reset()이 안불리면 return false한다.
		{
			return false;
		}
		return true;
	}

	void reset()
	{
        m_LastTime.exchange(GetTickCount());
	}

	/**
	@brief 49일까지 유효한 값을 반환해주는 경과값 함수
	@return 프로그램 시작한 이후 처음 메소드가 불려졌을 때부터 경과된 틱
	*/
	static DWORD GetTickLimit49Day()
	{
		static DWORD beginTick = GetTickCount();
		return GetTickCount() - beginTick;
	}

private:
	std::atomic<DWORD> m_LastTime;

	const DWORD m_ValidTime;

	/// 기본 체크 시간. 10분.
	static const DWORD DEFAULT_VALID_TIME = (60 * 1000) * 10; 
};

} //namespace light
