#include "light-view.h"
#include "FmodEx.h"
#include "Str.h"
#include <math.h>

namespace light
{

namespace view
{

FMOD::System * FmodEx::m_system = NULL;

FmodEx::FmodEx()
: m_load(false)
, m_sound(NULL)
, m_channel(NULL)
{
	for (int i = 0;i < 512;i++)
	{
		m_eq_map[i] = 100;
	}
}

FmodEx::~FmodEx()
{
	Release();
}

bool FmodEx::Initialize()
{
	if(m_system != NULL)
	{
		LOG_ERROR(L"already initialized.");
		return false;
	}

	// FMOD사운드시스템을 생성합니다.
	FMOD::System_Create(&m_system);
	m_system->init(32, FMOD_INIT_NORMAL, 0);
	return true;
}


bool FmodEx::Finalize()
{
	if(m_system == NULL)
	{
		LOG_ERROR(L"not initialized.");
		return false;
	}

	m_system->close();
	m_system->release();
	return true;
}

bool FmodEx::Load(const std::wstring& strFilename)
{
	FMOD_RESULT result = m_system->createSound(WCHAR_TO_CHAR(strFilename.c_str()), FMOD_DEFAULT, 0, &m_sound);
	if ( result != FMOD_OK )
	{
		LOG_ERROR(L"%s", FMOD_ErrorString(result));
		return false;
	}

	m_load = true;
	return true;
}

/**
@brief 소리파일을 불러옵니다.
*/
bool FmodEx::LoadStream(const std::wstring& strFilename, bool bLoop)
{
	FMOD_RESULT result = m_system->createStream(WCHAR_TO_CHAR(strFilename.c_str()), FMOD_DEFAULT, 0, &m_sound);
	if ( result != FMOD_OK )
	{
        LOG_ERROR(L"%s", FMOD_ErrorString(result));
        return false;
	}

	if(bLoop)
	{
		result = m_sound->setMode(FMOD_LOOP_NORMAL);
		if ( result != FMOD_OK )
		{
            LOG_ERROR(L"%s", FMOD_ErrorString(result));
            return false;
		}
	}

	m_load = true;
	return true;
}

bool FmodEx::Release()
{
	if ( m_sound == NULL )
		return false;

	FMOD_RESULT result = m_sound->release();
	if ( result != FMOD_OK )
	{
        LOG_ERROR(L"%s", FMOD_ErrorString(result));
        return false;
	}

	return true;
}

bool FmodEx::Play()
{
	if( false == m_load )
		return false;

	FMOD_RESULT result = m_system->playSound(m_sound, nullptr, false, &m_channel);
	if ( result != FMOD_OK )
	{
        LOG_ERROR(L"%s", FMOD_ErrorString(result));
        return false;
	}

	return true;
}

void FmodEx::Stop()
{
	if( true == IsPlaying() )
	{
		m_channel->stop();
	}
}

void FmodEx::Pause(bool Paused)
{
	m_channel->setPaused(Paused);
}

unsigned int FmodEx::GetTime()
{
	unsigned int ms;
	m_channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	return ms;
}

bool FmodEx::IsPlaying()
{
	bool Flag = FALSE;
	m_channel->isPlaying(&Flag);
	return Flag;
}

unsigned int FmodEx::GetLength()
{
	unsigned int lenms = 0;

	m_channel->getCurrentSound(&m_sound);
	m_sound->getLength(&lenms, FMOD_TIMEUNIT_MS);
	return lenms;
}

} // namespace view

} // namespace view
