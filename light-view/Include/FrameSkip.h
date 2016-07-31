#pragma once

namespace light
{

namespace view
{

class FrameSkip
{
public:
	FrameSkip(float& Fps)
		: m_fps(Fps)
		, m_timer(0.0f)
		, m_initialized(false)
		, m_using_qpf(false)
		, m_qpf_last_elapsed_time(0)
		, m_qpf_ticks_per_sec(0)
		, m_last_elapsed_time(0)
		, m_sec_per_frame(1.0f / Fps)
	{
	}

public:
	void Update()
	{
		m_timer += GetAppTimeDelay();		
	}

	void reset()
	{
		m_timer = 0.0f;
	}

	float Delta()
	{
		return m_sec_per_frame;
	}

	bool Frame() 
	{
		if( m_timer <= 0.0f )
		{
			return false;
		}
		// 한프레임에 해당하는 시간을 뺀다.
		m_timer -= m_sec_per_frame;
		return true;
	}

private:
	float GetAppTimeDelay()
	{
		// Initialize the timer
		if( false == m_initialized )
		{
			m_initialized = true;
			LARGE_INTEGER qwTicksPerSec;
			m_using_qpf = (QueryPerformanceFrequency( &qwTicksPerSec )==0) ? false : true;
			if( m_using_qpf )
			{
				m_qpf_ticks_per_sec = qwTicksPerSec.QuadPart;
				LARGE_INTEGER qwTime;
				QueryPerformanceCounter( &qwTime );
				m_qpf_last_elapsed_time = qwTime.QuadPart;
			}
		}

		double fTime;
		double fElapsedTime;
		if( m_using_qpf )
		{
			LARGE_INTEGER qwTime;
			QueryPerformanceCounter( &qwTime );

			// Return the elapsed time
			fElapsedTime = (double) ( qwTime.QuadPart - m_qpf_last_elapsed_time ) / (double) m_qpf_ticks_per_sec;
			m_qpf_last_elapsed_time = qwTime.QuadPart;
		} 
		else
		{
			// Get the time using timeGetTime()
			fTime = GetTickCount() * 0.001;

			// Return the elapsed time
			fElapsedTime = (double) (fTime - m_last_elapsed_time);
			m_last_elapsed_time = fTime;
		}
		return (float) fElapsedTime;
	}

private:	
	float&	m_fps;

	float	m_timer;

	bool	m_initialized;

	bool	m_using_qpf;

	__int64	m_qpf_last_elapsed_time;

	__int64	m_qpf_ticks_per_sec;

	double	m_last_elapsed_time;

	float	m_sec_per_frame;
};

} // namespace view

} // namespace view
