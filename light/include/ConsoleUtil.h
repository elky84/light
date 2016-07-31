#pragma once

namespace light
{

namespace ConsoleUtil
{

enum KEY
{
	EXT_KEY	= 0x000000E0,	//확장키 인식값 
	KEY_LEFT = 0x4B,
	KEY_RIGHT = 0x4D,
	KEY_UP = 0x48,
	KEY_DOWN = 0x50,
};


enum COLOR
{ 
	NONE = -1,
	BLACK,      /*  0 : 까망 */ 
	DARK_BLUE,    /*  1 : 어두운 파랑 */ 
	DARK_GREEN,    /*  2 : 어두운 초록 */ 
	DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */ 
	DARK_RED,    /*  4 : 어두운 빨강 */ 
	DARK_VOILET,  /*  5 : 어두운 보라 */ 
	DARK_YELLOW,  /*  6 : 어두운 노랑 */ 
	GRAY,      /*  7 : 회색 */ 
	DARK_GRAY,    /*  8 : 어두운 회색 */ 
	BLUE,      /*  9 : 파랑 */ 
	GREEN,      /* 10 : 초록 */ 
	SKY_BLUE,    /* 11 : 하늘 */ 
	RED,      /* 12 : 빨강 */ 
	VOILET,      /* 13 : 보라 */ 
	YELLOW,      /* 14 : 노랑 */ 	
	WHITE,      /* 15 : 하양 */
	END,
};

BOOL set_console_title(const std::wstring& strTitle);

void remove_close_button();

bool install_console_ctrl_handler();

void gotoxy(const WORD x, const WORD y);

void set_color(const WORD color, const WORD bgColor = BLACK);

int wherex();

int wherey();

/// 콘솔 윈도우 동작 키보드 단축키 동작 Disable
BOOL WINAPI DisableControlConsole(const DWORD event);

} // namespace ConsoleUtil

} //namespace light
