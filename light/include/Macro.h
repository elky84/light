#pragma once

#define _QUOTE(x)				L ## x
#define QUOTE(x)				_QUOTE(x)
#define _TO_STRING(x)			# x
#define TO_STRING(x)			QUOTE(_TO_STRING(x))

#define __LINEW__				TO_STRING(__LINE__)

///절대 값 매크로
#define ABS(a)                (((a) < (0))?-(a):(a))

///최대값 리턴 매크로
#define MAX(a,b)			  (((a) > (b)) ? (a) : (b))

///최소값 리턴 매크로
#define MIN(a,b)			  (((a) < (b)) ? (a) : (b))

///안전하게 변수 delete 하는 매크로
#define SAFE_DELETE(x)				if(x) {delete x; x = NULL;}

///안전하게 배열 delete 하는 매크로
#define SAFE_DELETE_ARRAY(x)		if(x) delete []x; x = NULL;

///안전하게 싱글턴 delete하는 매크로
#define SAFE_DELETE_SINGLETON(x)	x::Destroy()

///안전하게 릴리즈 하는 매크로
#define SAFE_RELEASE(x)				if(x) x->Release(); x = NULL;

///안전하게 DeleteObject하는 매크로
#define SAFE_DELTE_OBJECT(x)		if(x) DeleteObject(x);

///안전하게 CloseHandle하는 매크로
#define SAFE_CLOSE_HANDLE(x)		if(x != INVALID_HANDLE_VALUE) {CloseHandle(x); x = INVALID_HANDLE_VALUE;}

///안전하게 HWND를 CloseHandle하는 매크로
#define SAFE_DESTROY_HWND(x)		if(x != NULL) {DestroyWindow(x); x = NULL;}

///배열 범위 검사용 매크로. unsigned 용
#define IS_ARRAY_ACCESS(var, maxval) (0 <= var && var < maxval) ? true : false

///유효한 값인지 검사해서 true나 false를 리턴
#define IS_VALID_VALUE(var, minval, maxval) (minval <= var && var < maxval ) ? true : false

///함수 포인터 호출 매크로
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

///배열 갯수 계산 매크로
#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

template < bool >
struct CompileTimeChecker
{
	CompileTimeChecker(...){}
};

template <>
struct CompileTimeChecker< false >
{
};

#define STATIC_SIZE_CHECK( expr, msg )\
{\
class ERROR_##msg {}; \
	(void)sizeof(CompileTimeChecker< (expr) != 0 >((ERROR_##msg()))); \
}

#define STATIC_CHECK( expr, msg ) \
{ \
class error_##msg {}; \
	error_##msg _##msg; \
	CompileTimeChecker<(expr)!=0> _checker_##msg(_##msg); \
}

#define CLASS_DEFINITION( classname ) \
class Def##classname {}; \
	Def##classname _##classname; \
	CompileTimeChecker<(true)!=0> _checker_##classname(_##classname); 

#include <cassert>

#define ASSERT_MSG( expr, description )		assert(expr && description) 

#define ASSERT_ONLY_MSG( description )		assert(0 && description) 

#define UNIQUE_VAR(x)	safety_limit##x

static bool while_assert(bool bExpr)
{
	ASSERT_MSG(bExpr, "while_limit: exceeded iteration limit");
	return (bExpr);
}

#define while_limit(Limit, Expr) \
	ASSERT_MSG(Limit > 0, "while_limit: limit is zero or negative"); \
	int UNIQUE_VAR(__LINE__) = Limit; \
	while(Expr && while_assert(--UNIQUE_VAR(__LINE__) >= 0))

#define CHECK_EQUAL_RETURN(x) if(TO_STRING(x) == str) return x;

#define NonDerivableClass(x) \
class x; \
class __Cannot_Derive_Class_##x \
{ \
	friend class x; \
private: \
	__Cannot_Derive_Class_##x() {} \
	__Cannot_Derive_Class_##x(const __Cannot_Derive_Class_##x&) {} \
}
#define NonDerivableSuper(x)    public virtual __Cannot_Derive_Class_##x