#include "light.h"
#include "RaiseExceptions.h"

namespace light
{

void RaiseExceptions::PureVirtualFunctionCall()
{
	PureVirtualFunction::Child child;
}

void RaiseExceptions::CRTError()
{
	WCHAR szData[100000] = L"CRT Error Test !!";
	WCHAR szTargetBuffer[10] = L"";
	_tcsncpy_s(szTargetBuffer, szData, 10);
}

#pragma  warning(disable: 4718) // 스택 오버 플로우 일부러 냄
void RaiseExceptions::StackOverflow()
{
	WCHAR blockdata[1024 * 10000] = {0, };
	wcsncpy_s(blockdata, L"TestTestTest", 13);
	for(int n = 0; n < 100000; ++n)
	{
		StackOverflow();
	}
}

#pragma  warning(disable: 4718) // divide 0 경고를 막기 위함

void RaiseExceptions::NullPtrAccess()
{
	Object* pObject = new Child();
	pObject = NULL;
	wprintf_s(L"%d\n", pObject->Key());
}

#pragma  warning(disable: 4723) // divide 0 경고를 막기 위함

int Divide(const int nDivisor)
{
	return rand() / nDivisor;
}

#pragma  warning(default: 4723) // divide 0 경고를 막기 위함

void RaiseExceptions::DivideZero()
{
	int nResult = Divide(0); //Divide zero.
	wprintf_s(L"Result : %d\n", nResult);
}

void RaiseExceptions::InvalidCastingFunctionCall()
{
	Object* pObject = new Object;
	Child* pChild = static_cast<Child*>(pObject);

	pChild->set(53829532);

	delete pChild;
}

void RaiseExceptions::DuplicatedDelete()
{
	Object* pObject = new Object;
	delete pObject;
	delete pObject;
}

void RaiseExceptions::NullPtrAssign() //널 포인터 참조
{
	int* pPoint = 0;
	*pPoint = 1234;
}

// Vulnerable function
void vulnerable(const WCHAR *str) 
{
	WCHAR buffer[10] = {0, };
	memcpy(buffer, str, _tcslen(str)); // overrun buffer !!!
	wprintf_s(L"%s\n", buffer);
}

void RaiseExceptions::StackOverwrite()
{
	WCHAR large_buffer[] = L"This string is longer than 10 characters!! hahahahahahahahahahahahahaha";
	vulnerable(large_buffer);
}

void RaiseExceptions::DestructExceptionRaise()
{
	DestructorException exception;
}

void RaiseExceptions::ConstructorWithDestructExceptionRaise()
{
	DestructorException destroctor_exception;
	ConstructorException constroctor_exception;
}

int RecursiveFunctionCall(int s)
{
	// 컴파일러 속으라고 쓰레기코드
	int n = s+1;
	if (n<s)
		return n;
	else
		return RecursiveFunctionCall(n); 
}

void RaiseExceptions::UnlimitRecursiveFunctionCall()
{
	RecursiveFunctionCall(1);
}

void RaiseExceptions::HeabufferOverrun()
{
	std::string src = "1234567890 longer longer copy souce";
	char* buffer = new char[4];
	memcpy(buffer, src.c_str(), src.length());

	delete [] buffer;
}

} // namespace light