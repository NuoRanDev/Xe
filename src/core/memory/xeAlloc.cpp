#include "memory/xeAlloc.h"

namespace xe
{
	void xe_free(void* src) noexcept
	{
		return free(src);
	}
}