#include "stdafx.h"
#include "exception_test.h"

#include "RaiseExceptions.h"

exception_test::exception_test(const std::wstring& program_name)
	: light::EnvironmentManager(program_name)
{
	int n = 0;
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::PureVirtualFunctionCall);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::CRTError);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::StackOverflow);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::DivideZero);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::NullPtrAccess);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::DuplicatedDelete);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::InvalidCastingFunctionCall);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::NullPtrAssign);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::StackOverwrite);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::DestructExceptionRaise);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::ConstructorWithDestructExceptionRaise);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::UnlimitRecursiveFunctionCall);
	FUNCTION_ADD_TEST(++n, light::RaiseExceptions::HeabufferOverrun);

}

exception_test::~exception_test()
{
}

void exception_test::ProcessInput()
{
	_flushall();

	int nChar = 0;
	_tscanf_s(_T("%d"), &nChar);

	MAP_FUNC::iterator it = m_Func.find(nChar);
	if(it == m_Func.end())
	{
		LOG_INFO(_T("%s. You are invalid choice. Program to quit. Input[%d]"), __FUNCTIONW__, nChar);
		stop();
		return;
	}

	value_t& func = it->second;
	Call(func);
}

void exception_test::Call(value_t& func)
{
	func.second();
}

bool exception_test::initialize()
{ 
	LOG_INFO(_T("Exception Test."));
	for(MAP_FUNC::iterator it = m_Func.begin(); it != m_Func.end(); ++it)
	{
		value_t& value = it->second;
		LOG_INFO(_T("%d. %s"), it->first, value.first.c_str());
	}

	on_complete();
	return true;
}

void exception_test::on_update()
{
	Sleep(100);
	ProcessInput();
}

void exception_test::release()
{
}
