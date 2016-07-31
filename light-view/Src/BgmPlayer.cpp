#include "light-view.h"
#include "BgmPlayer.h"
#include "DirectoryUtil.h"

#include "XMLParser.h"

namespace light
{

namespace view
{

BgmPlayer::BgmPlayer()
{

}

BgmPlayer::~BgmPlayer()
{

}

bool BgmPlayer::Load(const std::wstring& strName)
{
	light::XMLParser parser;
	if ( false == parser.read_file(strName) ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName);
		return false;
	}

	if ( false == parser.execute(L"/BGMList") ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName);
		return false;
	}

	if ( false == parser.bind_elem(L"BGMInfo") ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName);
		return false;
	}

	std::wstring strDirectory = light::get_path_without_file_name(strName);

	long trackno = 0;
	if ( false == parser.bind_attrib(L"TrackNo", trackno) ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName);
		return false;
	}

	std::wstring file_name;
	if ( false == parser.bind_attrib(L"FileName", file_name) )
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName);
		return false;
	}

	while(parser.next())
	{
		SHARED_PTR_FMODEX Sound(new FmodEx);
		std::wstring strBGMFile = strDirectory + file_name;
		if( false == Sound->LoadStream(strBGMFile.c_str(), true))
		{
			LOG_ERROR(L"%s [%s:LoadFailed]", __FUNCTIONW__, file_name.c_str());
		}

		m_bgms.insert(MAP_BGM::value_type(trackno, Sound));
	}

	return true;
}

void BgmPlayer::Stop()
{
	if(m_current.get())
	{
		m_current->Stop();
		m_current.reset();
	}
}

bool BgmPlayer::Play(const long nTrackNo)
{
	MAP_BGM::iterator it = m_bgms.find(nTrackNo);
	if(it == m_bgms.end())
	{
		LOG_ERROR(L"%s NotFind BGM. TrackNo [%d]", __FUNCTIONW__, nTrackNo);
		return false;
	}

	if(m_current.get())
		m_current->Stop();

	m_current = it->second;
	m_current->Play();
	return true;
}

} // namespace view

} // namespace view
