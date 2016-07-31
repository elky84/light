#include "light.h"
#include "Icon.h"
#include <ShellAPI.h>

namespace light
{

/**
@brief 생성자
@param 아이콘 이름
*/
Icon::Icon(const std::wstring& name) 
	: m_hLarge(NULL)
	, m_hSmall(NULL)
	, m_name(name)
{
	::ExtractIconEx(name.c_str(), 0, &m_hLarge, &m_hSmall, 1);
}

/**
@brief 소멸자
*/
Icon::~Icon()
{
	::DestroyIcon(m_hLarge);
	::DestroyIcon(m_hSmall);
}

/**
@brief 아이콘 핸들 얻어오기 메소드
@return 아이콘 핸들
*/
HICON Icon::Large()
{
	return m_hLarge;
}

/**
@brief 아이콘 핸들 얻어오기 메소드
@return 아이콘 핸들
*/
HICON Icon::Small()
{
	return m_hSmall;
}

} // namespace light