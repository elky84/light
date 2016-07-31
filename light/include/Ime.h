#pragma once

/* Ime Header files */ 
#include <imm.h>
#include <objbase.h>
#include <dimm.h>

#pragma warning( disable : 4100 )

namespace light
{

class Ime
{
public:
	Ime();
	virtual ~Ime();

	bool initialize();

	INPUTCONTEXT*	LockIMC(HIMC hImc);
	BOOL			UnlockIMC(HIMC hImc);
	LPVOID			LockIMCC( HIMCC );
	BOOL			UnlockIMCC( HIMCC );

protected:

	HINSTANCE		m_hDllImm32;

	INPUTCONTEXT*	(WINAPI * _ImmLockIMC)( HIMC );
	BOOL			(WINAPI * _ImmUnlockIMC)( HIMC );
	LPVOID			(WINAPI * _ImmLockIMCC)( HIMCC );
	BOOL			(WINAPI * _ImmUnlockIMCC)( HIMCC );
};

} // namespace light