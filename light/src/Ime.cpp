#include "light.h"
#include "Ime.h"

namespace light
{

Ime::Ime() 
	: m_hDllImm32(NULL) 
{
}

Ime::~Ime()
{ 
	if(m_hDllImm32) 
		FreeLibrary(m_hDllImm32);
}

bool Ime::initialize()
{
	char path[MAX_PATH+1] = {0, };
	if( FALSE == GetSystemDirectoryA( path, MAX_PATH+1 ) ) 
		return false;

	strcat_s( path, "\\imm32.dll" );
	m_hDllImm32 = LoadLibraryA( path );
	if( m_hDllImm32 == NULL )
		return false;

	_ImmLockIMC	= (INPUTCONTEXT*(WINAPI*)(HIMC))GetProcAddress(m_hDllImm32, "ImmLockIMC");
	_ImmUnlockIMC = (BOOL(WINAPI*)(HIMC))GetProcAddress(m_hDllImm32, "ImmUnlockIMC");
	_ImmLockIMCC = (LPVOID(WINAPI*)(HIMCC))GetProcAddress(m_hDllImm32, "ImmLockIMCC");
	_ImmUnlockIMCC = (BOOL(WINAPI*)(HIMCC))GetProcAddress(m_hDllImm32, "ImmUnlockIMCC");
	return true;
}


INPUTCONTEXT* Ime::LockIMC(HIMC hImc)
{
	if(_ImmLockIMC == NULL) 
		return NULL;

	return _ImmLockIMC(hImc);
}


BOOL Ime::UnlockIMC(HIMC hImc)
{
	if(_ImmUnlockIMC == NULL) 
		return FALSE;

	return _ImmUnlockIMC(hImc);
}


LPVOID	Ime::LockIMCC( HIMCC himcc )
{
	if(_ImmLockIMCC == NULL) 
		return NULL;

	return _ImmLockIMCC(himcc);
}


BOOL Ime::UnlockIMCC( HIMCC himcc )
{
	if(_ImmUnlockIMCC == NULL) 
		return NULL;

	return _ImmUnlockIMCC(himcc);
}

} // namespace light