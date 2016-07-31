#pragma once

#define WM_TRAYICON_MSG		WM_USER + 100

namespace light
{

/**
@brief 트레이 아이콘 클래스
*/
class Tray
{
public:
	/**
	@brief 등록 메소드
	@param hWnd 윈도우 핸들
	@param hIcon 아이콘 핸들
	@param strTip 툴팁 텍스트
	*/
	static void set(HWND hWnd, HICON hIcon, const std::wstring& strTip)
	{
		// 트레이에 아이콘 등록
		NOTIFYICONDATA  NotifyIconData;
		NotifyIconData.cbSize = sizeof(NotifyIconData);
		NotifyIconData.hWnd = hWnd; // 메인 윈도우 핸들
		NotifyIconData.uID = IDR_MAINFRAME;  // 아이콘 리소스 ID
		NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
		NotifyIconData.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
		NotifyIconData.hIcon = hIcon; // 아이콘 로드 

		lstrcpy(NotifyIconData.szTip, strTip.c_str()); 
		Shell_NotifyIcon(NIM_ADD, &NotifyIconData);
		::SendMessage(hWnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)NotifyIconData.hIcon);

		ShowWindow(hWnd, SW_HIDE);
	}

	/**
	@brief 변경 메소드
	@param hWnd 윈도우 핸들
	@param hIcon 아이콘 핸들
	@param strTip 툴팁 텍스트
	*/
	static void Change(HWND hWnd, HICON hIcon, const std::wstring& strTip)
	{
		// 변경
		NOTIFYICONDATA NotifyIconData;
		NotifyIconData.cbSize = sizeof(NotifyIconData);
		NotifyIconData.hWnd = hWnd;
		NotifyIconData.uID = IDR_MAINFRAME;
		NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		NotifyIconData.uCallbackMessage = WM_TRAYICON_MSG;
		_tcscpy_s( NotifyIconData.szTip, _countof(NotifyIconData.szTip), strTip.c_str() );
		NotifyIconData.hIcon = hIcon;
		Shell_NotifyIcon( NIM_MODIFY, &NotifyIconData );
		::SendMessage(hWnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)NotifyIconData.hIcon);
	}

	/**
	@brief 삭제 메소드
	@param hWnd 윈도우 핸들
	*/
	static void Delete(HWND hWnd)
	{
		Release(hWnd);
		ShowWindow(hWnd, SW_SHOW);
	}

	/**
	@brief 릴리즈 메소드
	@param hWnd 윈도우 핸들
	*/
	static void Release(HWND hWnd)
	{
		NOTIFYICONDATA NotifyIconData;
		NotifyIconData.cbSize = sizeof(NotifyIconData);
		NotifyIconData.hWnd = hWnd;
		NotifyIconData.uID = IDR_MAINFRAME;
		Shell_NotifyIcon( NIM_DELETE, &NotifyIconData );
	}
};

} //namespace light
