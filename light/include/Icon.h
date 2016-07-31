#pragma once

namespace light
{

/**
@brief 아이콘 클래스
*/
class Icon
{
public:
	/// 생성자
	Icon(const std::wstring& name);

	/// 소멸자
	~Icon();

	/// 아이콘 큰거 핸들 얻어오기 메소드
	HICON Large();

	/// 아이콘 작은거 핸들 얻어오기 메소드
	HICON Small();

private:
	/// 아이콘 이름
	const std::wstring m_name;

	/// 아이콘 핸들
	HICON m_hLarge;

	/// 아이콘 핸들
	HICON m_hSmall;
};

} //namespace light
