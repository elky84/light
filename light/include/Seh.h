#pragma once

// 프로젝트 셋팅. 프로젝트 속성 > C/C++ > 코드 생성 > C++ 예외 처리 가능. /EHa (예, SEH 예외 있음(/EHa)) 선택
#include <malloc.h>

namespace light
{

// 사용자 정의 코드는 겹치지 않게 주의하라. 0x60000000 이상을 지정해야함
const DWORD EXCEPTION_USER_RAISED = 0XE0000001;

class CustomExceptionHandler
{
public:
	static BOOL Install();
};

class Seh
{
public:
	static void Install();

	static void RaiseException(const char* msg, ULONG_PTR arg1, ULONG_PTR arg2);

	DWORD GetErrorCode() const;

private:
	Seh(const DWORD exceptionCode);

	static void Translator(unsigned, EXCEPTION_POINTERS* info);

private:
	DWORD m_xception_code;
};

} // namespace light

#define SEH_TRY bool __stack_overflow = false; \
	bool __exception_occured = false; \
	try

#define SEH_CATCH catch(const light::Seh& seh) \
	{ \
		__exception_occured = true; \
		__stack_overflow = (seh.GetErrorCode() == EXCEPTION_STACK_OVERFLOW); \
	} \
	\
	if (__stack_overflow) \
		_resetstkoflw();\
	if (__exception_occured)