#include "light.h"
#include "Seh.h"

#include <process.h>
#include <stdlib.h>
#include <DbgHelp.h>

#include "MiniDump.h"

namespace light
{

unsigned int __stdcall DumpingThread(void* arg)
{
	MiniDumper::Instance()->WriteMiniDump((EXCEPTION_POINTERS*)arg);
	return 0;
}

void Dump(EXCEPTION_POINTERS* info)
{
	/// 오버 플로우 발생시에는 새로 스레드를 생성해서 덤프를 떠야한다.
	if ( info &&
		info->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		HANDLE handle = (HANDLE)::_beginthreadex(0, 0, DumpingThread, (void*)info, 0, NULL);
		if (handle)
		{
			WaitForSingleObject(handle, INFINITE);
			::CloseHandle(handle);
		}
	}
	else
	{
		MiniDumper::Instance()->WriteMiniDump(info);
	}
}

void PureCallHandler()
{
	Seh::RaiseException("PurecallHandler", 0, 0);
}

#if _MSC_VER >= 1400
void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	UNREFERENCED_PARAMETER(pReserved);

	static FILE* Fp = NULL;
	fopen_s(&Fp, "InvalidParameterHandler.log", "at");
	if(Fp != NULL)
	{
		fwprintf_s(Fp, L"Invalid parameter detected in function %s.\n", function);
		fwprintf_s(Fp, L"read_file: %s Line: %d\n", file, line);
		fwprintf_s(Fp, L"Expression: %s\n", expression);
		fclose(Fp);
	}

	_ASSERT(0);
	Seh::RaiseException("InvalidParameterHandler", 0, 0);
}

#endif // _MSC_VER >= 1400

LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* info)
{
	Dump(info);
	return EXCEPTION_CONTINUE_SEARCH;
}

#ifndef _M_IX86
#error "The following code only works for x86!"
#endif
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI CustomDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	UNREFERENCED_PARAMETER(lpTopLevelExceptionFilter);
	return NULL;
}

BOOL CustomExceptionHandler::Install()
{
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
	SetUnhandledExceptionFilter(&UnhandledExceptionHandler);

	HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
	if (hKernel32  == NULL) return FALSE;

	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(NULL == pOrgEntry) 
		return FALSE;

	unsigned char newJump[100];
	DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;

	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
	void *pNewFunc = &CustomDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD) pNewFunc;

	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

	newJump[0] = 0xE9;  // JMP absolute
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));

	SIZE_T bytesWritten;
	return WriteProcessMemory(GetCurrentProcess(),	pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
}

Seh::Seh(const DWORD exceptionCode)
: m_xception_code(exceptionCode)
{
}

DWORD Seh::GetErrorCode() const
{
	return m_xception_code;
}

void Seh::RaiseException(const char* msg, ULONG_PTR arg1, ULONG_PTR arg2)
{
	static const DWORD ARG_COUNT = 3;
	ULONG_PTR arg[ARG_COUNT] = { (ULONG_PTR)msg, arg1, arg2 };

	::RaiseException(EXCEPTION_USER_RAISED, 0, ARG_COUNT, arg);
}

void Seh::Install()
{
	_set_se_translator(Translator); 
	_set_purecall_handler(&PureCallHandler);

#if _MSC_VER >= 1400
	_set_invalid_parameter_handler(&InvalidParameterHandler);
#endif //  _MSC_VER >= 1400
}

void Seh::Translator(unsigned, EXCEPTION_POINTERS* info)
{
	Dump(info);
	throw Seh(info->ExceptionRecord->ExceptionCode);
}

} // namespace light
