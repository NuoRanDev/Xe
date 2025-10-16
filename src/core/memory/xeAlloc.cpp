#include "memory/xeAlloc.hpp"

namespace xe
{
	void xe_free(void* src) noexcept
	{
		return free(src);
	}
}