#include "stdafx.h"
#include "Main.h"
#include "WindowUtil.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	const std::wstring strClassName = L"3DTest";
	if( false == light::WindowUtil::register_class( strClassName, CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW, ::LoadIcon(GetModuleHandle(NULL), NULL) ) )
		return FALSE;

	RECT rect = {0, 0, 1024, 768};

	RUN_MAIN_DIRECTX(Main, strClassName, strClassName, rect);
	return 0;
}
