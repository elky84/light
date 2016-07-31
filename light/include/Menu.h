#pragma once

#include <Windows.h>
#include "Dispatcher.hpp"

namespace light
{

/**
@brief 팝업 메뉴 클래스
*/
class Popup
{
public:
	typedef boost::function<bool ()> func_t;

public:
	/// 생성자
	Popup(const HWND& hWnd);
	
	/// 소멸자
	~Popup();

	///팝업 추가
	bool Add(const std::wstring& name, func_t func);
	
	/// 구분자 추가
	bool Seperate();
	
	/// 보여주기
	bool Show();

	/// 숨기기
	bool Hide();
	
	/// 커맨드 콜백
	BOOL command(WORD nID);

private:
	///윈도우 핸들 참조자
	const HWND& m_hWnd;
	
	///팝업 메뉴 핸들
	HMENU m_hPopup;
	
	///식별 인덱스
	WORD m_AccIdx;

	///콜백 맵
	Dispatcher<WORD> m_dispatcher; 
};

} //namespace light  

