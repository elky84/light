#pragma once

#include "FmodEx.h"
#include "Singleton.h"

namespace light
{

namespace view
{

class BgmPlayer
	: public Singleton<BgmPlayer>
{
private:
	typedef std::tr1::shared_ptr<FmodEx> SHARED_PTR_FMODEX;
	typedef std::map<long, SHARED_PTR_FMODEX > MAP_BGM;

public:
	BgmPlayer();
	~BgmPlayer();

public:
	bool Load(const std::wstring& strName);

	bool Play(const long nTrackNo);

	void Stop();

private:
	MAP_BGM m_bgms;

	SHARED_PTR_FMODEX m_current;
};

} // namespace view

} // namespace view
