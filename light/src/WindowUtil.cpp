#include "light.h"
#include "WindowUtil.h"

#include "ImeInput.h"
#include "Input.h"

#include <boost/bind.hpp>

LRESULT CALLBACK lightWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return light::WindowUtil::WindowProc(hWnd, message, wParam, lParam);
}

namespace light
{

HWND WindowUtil::m_hWnd = NULL;

RECT WindowUtil::m_rect;

RECT WindowUtil::m_Resize;

bool WindowUtil::m_ActiveFlag = false;

Dispatcher<UINT, HWND, WPARAM, LPARAM> WindowUtil::m_dispatcher;

bool WindowUtil::AddDispatcher(UINT message, func_t func)
{
	if( false == m_dispatcher.register_func(message, func) )
	{
		LOG_ERROR(L"[%u]", message);
		return false;
	}
	return true;
}

bool WindowUtil::CallDispatcher(HWND hWnd, UINT message, WPARAM& wParam, LPARAM& lParam)
{
	return m_dispatcher.call(message, hWnd, wParam, lParam);
}

LRESULT WindowUtil::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( true == m_dispatcher.call(message, hWnd, wParam, lParam) )
	{
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND WindowUtil::CreateMainWindow(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags /* = WS_OVERLAPPEDWINDOW */, DWORD dwExflags /* = WS_EX_OVERLAPPEDWINDOW */, LPVOID lpParam /* = NULL */, HINSTANCE hInstance /* = ::GetModuleHandle */)
{
	m_hWnd = CreateEx(name, strClassName, rect, dwFlags, dwExflags, lpParam, hInstance);
	m_rect = m_Resize = rect;
	return m_hWnd;
}

HWND WindowUtil::CreateChildWindow(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags /* = WS_OVERLAPPEDWINDOW */, DWORD dwExflags /* = WS_EX_OVERLAPPEDWINDOW */, LPVOID lpParam /* = NULL */, HINSTANCE hInstance /* = ::GetModuleHandle */)
{
	return CreateEx(name, strClassName, rect, dwFlags, dwExflags, lpParam, hInstance);
}

HWND WindowUtil::CreateButton(HMENU hMenu, HWND hWnd, const std::wstring& name, const RECT& rect, LPVOID lpParam /* = NULL */, DWORD dwFlags /* = WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON */)
{
	return Create(hMenu, hWnd, L"button", rect, dwFlags, lpParam, name.c_str());
}

HWND WindowUtil::CreateListbox(HMENU hMenu, HWND hWnd, const RECT& rect, LPVOID lpParam /* = NULL */, DWORD dwFlags /* = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | LBS_STANDARD */)
{
	return Create(hMenu, hWnd, L"listbox", rect, dwFlags, lpParam);
}

HWND WindowUtil::CreateEditbox(HMENU hMenu, HWND hWnd, const RECT& rect, LPVOID lpParam /* = NULL */, DWORD dwFlags /* = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL */)
{
	return Create(hMenu, hWnd, L"edit", rect, dwFlags, lpParam);
}

void WindowUtil::Show(HWND hWnd, int nCmdShow /* = SW_SHOW */)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

bool WindowUtil::register_class(const std::wstring& strClassName, UINT uStyle /* = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW */, HICON hIcon /* = NULL */, HCURSOR hCursor /* = NULL */, HBRUSH hBrush /* = NULL */)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra			= 0;
	wndClass.cbSize				= sizeof(wndClass);
	wndClass.cbWndExtra			= DLGWINDOWEXTRA;

	wndClass.hbrBackground		= hBrush == NULL ? (HBRUSH)GetStockObject(WHITE_BRUSH) : hBrush;
	wndClass.hCursor			= hCursor == NULL ? LoadCursor(NULL, IDC_ARROW) : hCursor;
	wndClass.hIcon				= hIcon == NULL ? LoadIcon(NULL, IDI_APPLICATION) : hIcon;
	wndClass.hIconSm			= hIcon == NULL ? LoadIcon(NULL, IDI_APPLICATION) : hIcon;
	wndClass.hInstance			= GetModuleHandle(NULL);
	wndClass.lpszClassName		= strClassName.c_str();
	wndClass.lpfnWndProc		= &lightWindowProc;
	wndClass.lpszMenuName		= NULL;
	wndClass.style				= uStyle;

	if( FALSE == RegisterClassEx(&wndClass) )
		return false;

	return true;
}

void WindowUtil::SetWindowPos(HWND hWnd, const RECT& rect, UINT uFlags /* = SWP_NOMOVE */)
{
	::SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.right, rect.bottom, uFlags);
}

void WindowUtil::MoveWindowToCenter(HWND hWnd)
{
	//비 클라이언트의 영역 설정
	RECT clientSize = {MainWndRect().left, MainWndRect().top, MainWndRect().right - MainWndRect().left, MainWndRect().bottom - MainWndRect().top};

	int x = ( GetSystemMetrics(SM_CXSCREEN) - (clientSize.right  - clientSize.left) ) / 2;
	int y = ( GetSystemMetrics(SM_CYSCREEN) - (clientSize.bottom - clientSize.top ) ) / 2;

	RECT rect = {x, y, clientSize.right - clientSize.left, clientSize.bottom - clientSize.top};
	SetWindowPos(hWnd, rect, SWP_NOSIZE);
}

HWND WindowUtil::Create(HMENU hMenu, HWND hWnd, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags, LPVOID lpParam, const WCHAR* szWindowName /* = NULL */)
{
	RECT AdjustRect = rect;
	AdjustWindowRect(&AdjustRect, dwFlags, FALSE);

	return ::CreateWindow(strClassName.c_str(), szWindowName, dwFlags, AdjustRect.left, AdjustRect.top, AdjustRect.right - AdjustRect.left, AdjustRect.bottom - AdjustRect.top, hWnd, hMenu, NULL, lpParam);
}

HWND WindowUtil::CreateEx(const std::wstring& name, const std::wstring& strClassName, const RECT& rect, DWORD dwFlags /* = WS_OVERLAPPEDWINDOW */, DWORD dwExflags /* = WS_EX_OVERLAPPEDWINDOW */, LPVOID lpParam /* = NULL */, HINSTANCE hInstance /* = ::GetModuleHandle */)
{
	RECT AdjustRect = rect;
	AdjustWindowRect(&AdjustRect, dwFlags, FALSE);

	return ::CreateWindowEx(dwExflags, strClassName.c_str(), name.c_str(), dwFlags, AdjustRect.left, AdjustRect.top, AdjustRect.right - AdjustRect.left, AdjustRect.bottom - AdjustRect.top, NULL, NULL, hInstance, lpParam);
}

void WindowUtil::SetupCallbacks()
{
	AddDispatcher(WM_ACTIVATEAPP, boost::bind(&WindowUtil::ActiveApp, _1, _2, _3));

	AddDispatcher(WM_CREATE, boost::bind(&WindowUtil::Create, _1, _2, _3));

	AddDispatcher(WM_LBUTTONDBLCLK, boost::bind(&WindowUtil::LButtonDoubleClick, _1, _2, _3));
	AddDispatcher(WM_LBUTTONDOWN, boost::bind(&WindowUtil::LButtonDown, _1, _2, _3));
	AddDispatcher(WM_LBUTTONUP, boost::bind(&WindowUtil::LButtonUp, _1, _2, _3));
	AddDispatcher(WM_RBUTTONDBLCLK, boost::bind(&WindowUtil::RButtonDoubleClick, _1, _2, _3));
	AddDispatcher(WM_RBUTTONDOWN, boost::bind(&WindowUtil::RButtonDown, _1, _2, _3));
	AddDispatcher(WM_RBUTTONUP, boost::bind(&WindowUtil::RButtonUp, _1, _2, _3));
	AddDispatcher(WM_MOUSEWHEEL, boost::bind(&WindowUtil::Wheel, _1, _2, _3));
	AddDispatcher(WM_MOUSEMOVE, boost::bind(&WindowUtil::MouseMove, _1, _2, _3));

	AddDispatcher(WM_KEYDOWN, boost::bind(&WindowUtil::KeyDown, _1, _2, _3));
	AddDispatcher(WM_KEYUP, boost::bind(&WindowUtil::KeyUp, _1, _2, _3));

	AddDispatcher(WM_CHAR, boost::bind(&WindowUtil::Char, _1, _2, _3));

	AddDispatcher(WM_SIZE, boost::bind(&WindowUtil::get_size, _1, _2, _3));

	AddDispatcher(WM_IME_NOTIFY, boost::bind(&WindowUtil::ImeNotify, _1, _2, _3));
	AddDispatcher(WM_IME_COMPOSITION, boost::bind(&WindowUtil::ImeComposition, _1, _2, _3));
	AddDispatcher(WM_IME_STARTCOMPOSITION, boost::bind(&WindowUtil::ImeStartComposition, _1, _2, _3));
	AddDispatcher(WM_IME_ENDCOMPOSITION, boost::bind(&WindowUtil::ImeEndComposition, _1, _2, _3));
	AddDispatcher(WM_INPUTLANGCHANGE, boost::bind(&WindowUtil::InputLangChange, _1, _2, _3));
}

void WindowUtil::WindowToScreen(HWND hwnd, POINT* Point)
{
	RECT Rect;
	GetWindowRect( hwnd, &Rect );
	Point->x = Point->x + Rect.left;
	Point->y = Point->y + Rect.top;
}

void WindowUtil::ScreenToWindow(HWND hwnd, POINT* Point)
{
	RECT Rect;
	GetWindowRect( hwnd, &Rect );
	Point->x = Point->x - Rect.left;
	Point->y = Point->y - Rect.top;
}

void WindowUtil::WindowToClient(HWND hwnd, POINT* Point)
{
	WindowToScreen( hwnd, Point );
	ScreenToClient( hwnd, Point );
}

void WindowUtil::ClientToWindow(HWND hwnd, POINT* Point)
{
	ClientToScreen( hwnd, Point );
	ScreenToWindow( hwnd, Point );
}

bool WindowUtil::LButtonDoubleClick(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::LButtonDoubleClick(true);
	return true;
}

bool WindowUtil::LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::LButtonClick(true);
	Input::Mouse::LButtonClickPos(Input::Mouse::Pos());
	return true;
}

bool WindowUtil::LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::LButtonClick(false);
	Input::Mouse::LButtonDoubleClick(false);
	return true;
}

bool WindowUtil::RButtonDoubleClick(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::RButtonDoubleClick(true);
	return true;
}

bool WindowUtil::RButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::RButtonClick(true);
	Input::Mouse::RButtonClickPos(Input::Mouse::Pos());
	return true;
}

bool WindowUtil::RButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::RButtonClick(false);
	Input::Mouse::RButtonDoubleClick(false);
	return true;
}

bool WindowUtil::Wheel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Mouse::Wheel((GET_WHEEL_DELTA_WPARAM(wParam) < 0)?-1:1);
	return true;
}

bool WindowUtil::MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT Point = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
	Point.x = (LONG)((float)m_rect.right / m_Resize.right * Point.x);
	Point.y = (LONG)((float)m_rect.bottom / m_Resize.bottom * Point.y);

	Input::Mouse::Pos(light::POSITION_INT(Point.x, Point.y));
	return false;
}

bool WindowUtil::KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Keyboard::Up(wParam);
	return false;
}

bool WindowUtil::KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Input::Keyboard::Down(wParam);
	return false;
}

bool WindowUtil::get_size(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_Resize.right = LOWORD(lParam);
	m_Resize.bottom = HIWORD(lParam);
	return false;
}

bool WindowUtil::Char(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return ImeInput::Instance()->OnChar(hWnd, wParam, lParam);
}

bool WindowUtil::ImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return ImeInput::Instance()->OnNotify(hWnd, wParam, lParam);
}

bool WindowUtil::ImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return ImeInput::Instance()->OnComposition(hWnd, wParam, lParam);
}

bool WindowUtil::ImeStartComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool WindowUtil::ImeEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return ImeInput::Instance()->OnEndComposition(hWnd, wParam, lParam);
}

bool WindowUtil::InputLangChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ImeInput::Instance()->OnInputLanguageChange(hWnd, wParam, lParam);
	return false;
}

bool WindowUtil::Create(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ImeInput::Instance()->InitInput();
	ImeInput::Instance()->OnInputLanguageChange(hWnd, 0, (LPARAM)GetKeyboardLayout(0));
	return false;
}

bool WindowUtil::ActiveApp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_ActiveFlag = wParam ? true : false;
	if( m_ActiveFlag == false)
	{
		Input::Keyboard::reset();
		Input::Mouse::reset();
	}
	return true;
}

} //namespace light
