#include "light.h"
#include "Menu.h"

namespace light
{

Popup::Popup(const HWND& hWnd) 
: m_hWnd(hWnd)
, m_hPopup(CreatePopupMenu())
, m_AccIdx(WM_USER + 500)
{

}

Popup::~Popup()
{
	DestroyMenu(m_hPopup);
}

bool Popup::Add(const std::wstring& name, func_t func)
{
	if( false == AppendMenu(m_hPopup, MF_STRING, m_AccIdx, name.c_str()))
	{
		LOG_ERROR(L"Error. [%d, %s]", m_AccIdx, name.c_str());
		return false;
	}

	m_dispatcher.register_func(m_AccIdx, func);
	m_AccIdx++;
	return true;
}

bool Popup::Seperate()
{
	if( false == AppendMenu(m_hPopup, MF_SEPARATOR, NULL, NULL))
	{
		LOG_ERROR(L"Error.");
		return false;
	}
	return true;
}

bool Popup::Show()
{
	if( FALSE == SetForegroundWindow(m_hWnd))
		return false;

	POINT pt;
	GetCursorPos(&pt);
	BOOL nID = TrackPopupMenu(m_hPopup, TPM_LEFTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);
	if(nID == 0)
		return false;

	command((WORD)nID);
	return true;
}

bool Popup::Hide()
{
	if( TRUE == SetForegroundWindow(m_hWnd))
		return false;

	ShowWindow((HWND)m_hPopup, SW_HIDE);
	return true;
}

BOOL Popup::command(WORD nID)
{
	return m_dispatcher.call(nID);
}

} //namespace light
