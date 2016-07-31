#include "light.h"
#include "HotKey.h"

namespace light
{

DWORD HotKey::m_CountableID = WM_USER + 500;

HotKey::HotKey(const HWND& hWnd)
: m_hWnd(hWnd)
, m_ID(++m_CountableID)
, m_register(false)
{
}

HotKey::~HotKey()
{
}

bool HotKey::set(const INFORMATION& Information)
{
	if( true == m_register)
	{
		LOG_ERROR(L"Hwnd [%d] CtrlID [%u] Modifier[%u] Vk[%u]", m_hWnd, m_ID, Information.Modifier(), Information.VirtualKey());
		return false;
	}

	if( FALSE == RegisterHotKey(m_hWnd, m_ID, Information.Modifier(), Information.VirtualKey()) )
	{
		LOG_ERROR(L"Code [%d] Hwnd [%d] CtrlID [%u] Modifier[%u] Vk[%u]", GetLastError(), m_hWnd, m_ID, Information.Modifier(), Information.VirtualKey());
		return false;
	}

	m_Information = Information;
	m_register = true;
	return true;
}

bool HotKey::set()
{
	if(m_Information.Valid() == false)
	{
		return true;
	}

	if( true == m_register )
	{
		LOG_ERROR(L"Hwnd [%d] CtrlID [%u] Modifier[%u] Vk[%u]", m_hWnd, m_ID, m_Information.Modifier(), m_Information.VirtualKey());
		m_Information.initialize();
		return false;
	}

	if( FALSE == RegisterHotKey(m_hWnd, m_ID, m_Information.Modifier(), m_Information.VirtualKey()) )
	{
		LOG_ERROR(L"Code [%d] Hwnd [%d] CtrlID [%u] Modifier[%u] Vk[%u]", GetLastError(), m_hWnd, m_ID, m_Information.Modifier(), m_Information.VirtualKey());
		m_Information.initialize();
		return false;
	}

	m_register = true;
	return true;
}

bool HotKey::clear()
{
	if(m_Information.Valid() == false)
	{
		return true;
	}

	if( false == m_register )
	{
		LOG_ERROR(L"Not registered HotKey. Hwnd [%d] CtrlID [%u]", GetLastError(), m_hWnd, m_ID);
		return false;
	}

	if( FALSE == UnregisterHotKey(m_hWnd, m_ID) )
	{
		LOG_ERROR(L"Code [%d] Hwnd [%d] CtrlID [%u] Modifier[%u] Vk[%u]", GetLastError(), m_hWnd, m_ID, m_Information.Modifier(), m_Information.VirtualKey());
		return false;
	}

	m_register = false;
	return true;
}

bool HotKey::Clear()
{
	if( false == clear() )
	{
		return false;
	}
	m_Information.initialize();
	return true;
}

std::wstring HotKey::VirtualKeyToString(const UINT vk) 
{
	WCHAR buf[256] = L"";
	unsigned int scanCode = MapVirtualKey(vk, 0);
	switch (vk) 
	{ 
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys 
	case VK_PRIOR: case VK_NEXT: // page up and page down 
	case VK_END: case VK_HOME: 
	case VK_INSERT: case VK_DELETE: 
	case VK_DIVIDE: // numpad slash 
	case VK_NUMLOCK: 
		scanCode |= 0x100; // set extended bit 
		break; 
	}

	int nRet = GetKeyNameText(scanCode << 16, buf, 256);
	if( 0 == nRet)
	{
		LOG_ERROR(L"GetKeyNameText() Failed Code [%d]", GetLastError());
	}
	return std::wstring(buf);
}

std::wstring HotKey::InformationToString(const INFORMATION& Information) 
{
	if(Information.VirtualKey() == 0)
		return L"";

	std::wstringstream strStream = ModifierToString(Information.Modifier());
	if( false == strStream.str().empty() )
		strStream << L"+ ";

	strStream << VirtualKeyToString(Information.VirtualKey());
	return strStream.str();
}

std::wstringstream HotKey::ModifierToString(const MODIFIER Modifier) 
{
	std::wstringstream strStream;
	if( Modifier & MODIFIER_ALT)
		strStream << L"Alt ";

	if( Modifier &  MODIFIER_CONTROL)
		strStream << L"Ctrl ";

	if( Modifier &  MODIFIER_SHIFT)
		strStream << L"Shift ";

	if( Modifier & MODIFIER_WIN)
		strStream << L"Win ";

	return strStream;
}

} //namespace light
