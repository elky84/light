#pragma once

#include "light.h"

namespace light
{

class LFH
{
public:
	/// 프로세스 힙에 LFH 적용
	static bool ApplyProcessHeap();

	/// CRT 힙에 LFH 적용
	static bool ApplyCrtHeap();

	/// 프로세스의 모든 힙에 적용
	static bool ApplyAllHeap();

private:
	static bool ApplyHeap(HANDLE hHeap);
};

} //namespace light
