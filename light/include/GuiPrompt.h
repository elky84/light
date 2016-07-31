#pragma once

#include "PromptInterface.h"
#include "WindowUtil.h"

namespace light
{

#define IDC_PROMPT_EDITBOX	(WM_USER + 2000)

#define IDC_PROMPT_BUTTON	(WM_USER + 2001)

#define IDC_PROMPT_LISTBOX	(WM_USER + 2002)

class GuiPrompt 
    : public PromptInterface
{
public:
	GuiPrompt(const end_callback end, const std::wstring& notice = L"Prompt");

	virtual ~GuiPrompt();

	virtual void stop();

	virtual void notice(const std::wstring& notice);
	
	virtual void callback(const command_callback callback);

	bool list_box_message(const std::wstring& str);

private:
    virtual void begin();

	void process_window(HWND hWnd);

	void get_message();

	virtual void on_update();

    void wrapped_stop();

private: //윈도우 콜백

	bool on_key_up(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool on_command(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool on_close(HWND hWnd, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;

	HWND m_hEditbox;

	HWND m_hButton;

	HWND m_hReservedFocus;

	HWND m_hListBox;
};

} //namespace light
