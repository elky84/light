#pragma once

#include <queue>

#include <fmod_errors.h>
#include <fmod.hpp>

#pragma comment(lib, "fmodex_vc.lib")

namespace light
{

namespace view
{

/// @brief 음악이나 효과음등을 불러와 재생하고 관리합니다
class FmodEx
{
public:
	FmodEx();
	~FmodEx();

	static bool Initialize();

	static bool Finalize();

	bool Load(const std::wstring& strFilename);

	bool LoadStream(const std::wstring& strFilename, bool bLoop = false);

	bool Release();

	bool Play();

	void Stop();

	void Pause(bool Paused);

	void Update();

	unsigned int GetTime();

	bool IsPlaying();

	unsigned int GetLength();

	static FMOD::System* System() 
	{
		return m_system;
	}

private:
	static FMOD::System* m_system;

private:
	FMOD::Channel* m_channel;

	FMOD::Sound* m_sound;

	int	m_eq_map[512];

	bool m_load;
};

} // namespace view

} // namespace view
