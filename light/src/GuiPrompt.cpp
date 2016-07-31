#include "light.h"
#include "GuiPrompt.h"

#include <boost/bind.hpp>

namespace light
{

GuiPrompt::GuiPrompt(const end_callback end, const std::wstring& notice)
: PromptInterface(end, notice)
, m_hWnd(NULL)
, m_hEditbox(NULL)
, m_hButton(NULL)
, m_hReservedFocus(NULL)
{
	WindowUtil::AddDispatcher(WM_PAINT, boost::bind(&GuiPrompt::on_paint, this, _1, _2, _3));
	WindowUtil::AddDispatcher(WM_CLOSE, boost::bind(&GuiPrompt::on_close, this, _1, _2, _3));
	WindowUtil::AddDispatcher(WM_COMMAND, boost::bind(&GuiPrompt::on_command, this, _1, _2, _3));
	WindowUtil::AddDispatcher(WM_KEYUP, boost::bind(&GuiPrompt::on_key_up, this, _1, _2, _3));
}

GuiPrompt::~GuiPrompt()
{
	SAFE_DESTROY_HWND(m_hEditbox);
	SAFE_DESTROY_HWND(m_hButton);
	SAFE_DESTROY_HWND(m_hListBox);
	SAFE_DESTROY_HWND(m_hWnd);
}

void GuiPrompt::begin()
{
    if (false == WindowUtil::register_class(L"GuiPrompt"))
    {
        stop();
        return;
    }

    {
        RECT rect = { 5, 25, 240, 280 };
        m_hWnd = WindowUtil::CreateMainWindow(m_notice, L"GuiPrompt", rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, WS_EX_OVERLAPPEDWINDOW, this);
        process_window(m_hWnd);
    }

    {
        RECT rect = { 10, 5, 220, 25 };
        m_hEditbox = WindowUtil::CreateEditbox((HMENU)IDC_PROMPT_EDITBOX, m_hWnd, rect, this);
        process_window(m_hEditbox);
        ::SetFocus(m_hEditbox);
    }

    {
        RECT rect = { 10, 30, 220, 50 };
        m_hButton = WindowUtil::CreateButton((HMENU)IDC_PROMPT_BUTTON, m_hWnd, L"command", rect, this);
        process_window(m_hButton);

        SendMessage(m_hWnd, DM_SETDEFID, IDC_PROMPT_BUTTON, 0);
    }

    {
        RECT rect = { 10, 55, 220, 250 };
        m_hListBox = WindowUtil::CreateListbox((HMENU)IDC_PROMPT_LISTBOX, m_hWnd, rect, this);
        process_window(m_hListBox);
    }
}

void GuiPrompt::stop()
{
    if (m_hWnd != NULL)
    {
        PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    }

    PromptInterface::stop();
}
	
void GuiPrompt::notice(const std::wstring& notice)
{
	SetWindowText(m_hWnd, m_notice.c_str());
	__super::notice(notice);
}
	
void GuiPrompt::callback(const command_callback callback)
{
	__super::callback(callback);
	m_hReservedFocus = m_hEditbox;
}

void GuiPrompt::process_window(HWND hWnd)
{
	if(hWnd == NULL)
	{
		stop();
	}
	else
	{
		WindowUtil::Show(hWnd);
	}
}

void GuiPrompt::get_message()
{
    MSG msg;
    if (::GetMessage(&msg, m_hWnd, 0, 0))
    {
		if ( false == WindowUtil::CallDispatcher(msg.hwnd, msg.message, msg.wParam, msg.lParam) )
		{
			::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
		}
	}
}

void GuiPrompt::on_update()
{
	if(NULL != m_hReservedFocus)
	{
        ::SetFocus(m_hReservedFocus);
		m_hReservedFocus = NULL;
	}
	get_message();
}

bool GuiPrompt::on_key_up(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	WCHAR szText[128] = L"";
	if(hWnd == m_hEditbox)
	{
		DWORD vk = wParam;
		if( VK_RETURN == vk )
		{
			call();
            ::SetWindowText(m_hEditbox, szText);
		}
	}
	return false;
}

bool GuiPrompt::on_command(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);

	WCHAR szText[128] = L"";
	switch(LOWORD(wParam))
	{
	case IDC_PROMPT_BUTTON:
		{
			if(HIWORD(wParam) == BN_CLICKED)
			{
				list_box_message(m_command);

				call();

                ::SetWindowText(m_hEditbox, szText);
			}
		}
		break;
	case IDC_PROMPT_EDITBOX:
		{
			if(HIWORD(wParam) == EN_CHANGE)
			{
                ::GetWindowText((HWND)lParam, szText, 128);
				m_command = szText;
			}
		}
		break;
	}
	return false;
}

bool GuiPrompt::on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	UpdateWindow(hWnd);
	return false;
}

bool GuiPrompt::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if(m_hWnd == hWnd)
	{
		DestroyWindow(hWnd);
        m_hWnd = NULL;
        stop();
    }
	return true;
}	


bool GuiPrompt::list_box_message(const std::wstring& str)
{
	return 0 == SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

} //namespace light
