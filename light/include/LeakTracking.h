#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC	// 메모리 누수 탐지를 위해서 선언해 주어야 한다.

#include <crtdbg.h>
#include <cstdlib>

namespace light
{

class LeakTracking
{
public:
	LeakTracking()
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// 콘솔 프로그램일 경우
#ifdef _CONSOLE
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
#endif	// _CONSOLE

		// mfc 사용하는 프로젝트는 DEBUG_NEW 사용하세요.
#if defined (__AFX_H__)

#ifndef new
#define new DEBUG_NEW 
#endif // new

#else // __AFX_H__

#define DEBUG_NORMALBLOCK new(_NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef new
#undef new
#endif // new

#define new DEBUG_NORMALBLOCK

#endif // __AFX_H__

#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ))

#ifdef calloc
#undef calloc
#endif
#define calloc(c, s)      _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef realloc
#undef realloc
#endif
#define realloc(p, s)     _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef _expand
#undef _expand
#endif
#define _expand(p, s)     _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef free
#undef free
#endif
#define free(p)           _free_dbg(p, _NORMAL_BLOCK)

#ifdef _msize
#undef _msize
#endif
#define _msize(p)         _msize_dbg(p, _NORMAL_BLOCK)
	}

	static int CheckMemory()			
	{
		return _CrtCheckMemory();
	}

	static void BreakAlloc(long ncrtBreakAlloc)
	{
		//_crtBreakAlloc 변수를 직접 설정해도 됨.
		_CrtSetBreakAlloc(ncrtBreakAlloc);
	}
};

} // namespace light

#endif	// _DEBUG