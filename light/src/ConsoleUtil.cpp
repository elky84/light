#include "light.h"
#include "ConsoleUtil.h"

namespace light
{

namespace ConsoleUtil
{

BOOL set_console_title(const std::wstring& strTitle)
{
	return ::SetConsoleTitle(strTitle.c_str());
}

void remove_close_button()
{
	RemoveMenu(::GetSystemMenu(GetConsoleWindow(), FALSE),  SC_CLOSE, MF_BYCOMMAND);
}

bool install_console_ctrl_handler()
{
	if ( FALSE == SetConsoleCtrlHandler(DisableControlConsole, TRUE) )
	{
		wprintf_s(L"Unable to install handler!\n");
		return false;
	}
	return true;
}

BOOL WINAPI DisableControlConsole(const DWORD event)
{
	switch (event)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT: 
		return TRUE; 
	default: 
		return FALSE;
	}
}

void set_color(const WORD Color, const WORD bgColor) 
{ 
	WORD color = Color & 0xf;
	WORD bgcolor = bgColor & 0xf;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void gotoxy(const WORD x, const WORD y) 
{ 
	COORD Cur; 
	Cur.X=x; 
	Cur.Y=y; 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur); 
} 

int wherex() 
{ 
	CONSOLE_SCREEN_BUFFER_INFO BufInfo; 
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo); 
	return BufInfo.dwCursorPosition.X; 
} 

int wherey() 
{ 
	CONSOLE_SCREEN_BUFFER_INFO BufInfo; 
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo); 
	return BufInfo.dwCursorPosition.Y; 
} 

} // namespace ConsoleUtil

} // namespace light