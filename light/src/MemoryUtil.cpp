#include "light.h"
#include "MemoryUtil.hpp"

namespace light
{

/// 프로세스 힙에 LFH 적용
bool LFH::ApplyProcessHeap()
{
	return ApplyHeap(GetProcessHeap());
}

/// CRT 힙에 LFH 적용
bool LFH::ApplyCrtHeap()
{
	intptr_t hCrtHeap = _get_heap_handle();	
	return ApplyHeap((PVOID)hCrtHeap);
}

/// 프로세스의 모든 힙에 적용
bool LFH::ApplyAllHeap()
{
	HANDLE Heaps[1025];
	DWORD Count = GetProcessHeaps( 1024, Heaps );
	for( DWORD i = 0; i < Count; ++i )
	{
		if( false == ApplyHeap(Heaps[i]) )
		{
			return false;
		}
	}
	return true;
}

bool LFH::ApplyHeap(HANDLE hHeap)
{
#ifdef _DEBUG
	LOG_ERROR(L"DebugMode is can't ApplyHeap. %p", hHeap);
	return false;
#else // _DEBUG
	// 디버거 attach 중이면, 이미 적용된 것처럼 인정한다.
	if( TRUE == IsDebuggerPresent() )
	{
		return true;
	}

	// http://www.gpgstudy.com/forum/viewtopic.php?p=119181
	// MSDN : HeapQueryInformation http://msdn.microsoft.com/en-us/library/aa366703%28v=vs.85%29.aspx
	ULONG HeapInfo = 0;
	if( FALSE == HeapQueryInformation(hHeap, HeapCompatibilityInformation, &HeapInfo, sizeof(HeapInfo), NULL))
	{
		LOG_ERROR(L"HeapQueryInformation() Failed. Err[%u] Handle[%p]", GetLastError(), hHeap);
		return false;
	}

	const ULONG HEAP_LFH = 2;
	if( HEAP_LFH == HeapInfo )
	{
		// already used LFH
		return true;
	}

	// MSDN : HeapSetInformation http://msdn.microsoft.com/en-us/library/aa366705%28v=vs.85%29.aspx
	ULONG ulEnableLFH = 2;
	if ( FALSE == HeapSetInformation(hHeap, HeapCompatibilityInformation, &ulEnableLFH, sizeof(ulEnableLFH)))
	{
		LOG_ERROR(L"HeapSetInformation() Failed. Err[%u] Handle[%p]", GetLastError(), hHeap);
		return false;
	}
	return true;
#endif //_DEBUG
}

} //namespace light
