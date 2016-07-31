#pragma once

#include <Windows.h>
#include "Dispatcher.hpp"

#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK lightWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#define KEY_DOWN(vkCode) ((GetAsyncKeyState(vkCode) & 0x8000) ? 1 : 0)
#define KEY_UP(vkCode) ((GetAsyncKeyState(vkCode) & 0x8000) ? 0 : 1)

namespace light
{

class WindowUtil
{
public:
	typedef boost::function<bool (HWND, WPARAM, LPARAM)> func_t;

public:
	static HWND& MainHWND()
	{
		return m_hWnd;
	}

	static const RECT& MainWndRect()
	{
		return m_rect;
	}

public:
	static LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static HWND CreateMainWindow(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags = WS_OVERLAPPEDWINDOW, DWORD dwExflags = WS_EX_OVERLAPPEDWINDOW, LPVOID lpParam = NULL, HINSTANCE hInstance = ::GetModuleHandle(NULL));

	static HWND CreateChildWindow(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags = WS_OVERLAPPEDWINDOW, DWORD dwExflags = WS_EX_OVERLAPPEDWINDOW, LPVOID lpParam = NULL, HINSTANCE hInstance = ::GetModuleHandle(NULL));

	static HWND CreateButton(HMENU hMenu, HWND hWnd, const std::wstring& name, const RECT& rect, LPVOID lpParam = NULL, DWORD dwFlags = WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON);

	static HWND CreateEditbox(HMENU hMenu, HWND hWnd, const RECT& rect, LPVOID lpParam = NULL, DWORD dwFlags = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL);

	static HWND CreateListbox(HMENU hMenu, HWND hWnd, const RECT& rect, LPVOID lpParam = NULL, DWORD dwFlags = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | LBS_STANDARD);

	static void Show(HWND hWnd, int nCmdShow = SW_SHOW);

	static bool register_class(const std::wstring& strClassName, UINT uStyle = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW, HICON hIcon = NULL, HCURSOR hCursor = NULL, HBRUSH hBrush = NULL);

	static void SetWindowPos(HWND hWnd, const RECT& rect, UINT uFlags = SWP_NOMOVE);

	static void MoveWindowToCenter(HWND hWnd);

	static void SetupCallbacks();

public: //Dispatcher 기능
	static bool AddDispatcher(UINT message, func_t func);

	static bool CallDispatcher(HWND hWnd, UINT message, WPARAM& wParam, LPARAM& lParam);

public: // Util
	static void WindowToScreen(HWND hwnd, POINT* Point);

	static void ScreenToWindow(HWND hwnd, POINT* Point);

	static void WindowToClient(HWND hwnd, POINT* Point);

	static void ClientToWindow(HWND hwnd, POINT* Point);

private: // WINDOWS 콜백
	static bool LButtonDoubleClick(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool RButtonDoubleClick(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool RButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool RButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool Wheel(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool Create(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool Char(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool get_size(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool ImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static bool ImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static bool ImeStartComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static bool ImeEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static bool InputLangChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

private:
	static bool ActiveApp(HWND hWnd, WPARAM wParam, LPARAM lParam);

private:
	static HWND Create(HMENU hMenu, HWND hWnd, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags, LPVOID lpParam, const WCHAR* szWindowName = NULL);

	static HWND CreateEx(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags = WS_OVERLAPPEDWINDOW, DWORD dwExflags = WS_EX_OVERLAPPEDWINDOW, LPVOID lpParam = NULL, HINSTANCE hInstance = ::GetModuleHandle(NULL));

public:
	static bool is_active()
	{
		return m_ActiveFlag;
	}

private:
	static HWND m_hWnd;

	static RECT m_rect;

	static RECT m_Resize;

	static bool m_ActiveFlag;

	static Dispatcher<UINT, HWND, WPARAM, LPARAM> m_dispatcher;
};

} //namespace light
