#include "stdafx.h"
#include "NWUtil.h"

namespace light
{

namespace Math
{

int PositionalNumber(int num, int radix = 10)
{
	//123 -> 1, 12 -> 2, 1 -> 3, 0
	int count = 0;
	while(num > 0)
	{
		num /= radix;
		count++;
	}

	return count;
}

} // namespace Math

} // namespace NW