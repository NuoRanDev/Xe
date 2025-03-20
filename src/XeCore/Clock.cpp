import xe.xeCore.Clock;

import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeOrdinals;

#if defined(_WIN32)
#include <Windows.h>
#endif // OS hearder

namespace xe
{
	xeInt64 Clock::GetCurTime() const
	{
#if defined(_WIN32)
		LARGE_INTEGER _time;
		QueryPerformanceCounter(&_time);
		return _time.QuadPart;
#elif defined(__linux__)
#endif // OS Time api 
	}
}