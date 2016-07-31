#pragma once

namespace light
{

/**
@brief 고해상도 퍼포먼스 측정기
*/
class PerfomanceCounter
{
public:
	///생성자
	PerfomanceCounter();

	///소멸자
	~PerfomanceCounter();
	
	void begin();

	void end();

	double GetElapsedSecond();

	DWORD GetElapsedMilisecond();

private:
	LARGE_INTEGER m_begin_counter;

	LARGE_INTEGER m_end_counter;
	
	LARGE_INTEGER m_frequency;

};

} //namespace light
