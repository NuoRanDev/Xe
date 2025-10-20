#include "memory/xeAlloc.hpp"

namespace xe
{
	void xe_free(void* src) noexcept
	{
		if (src != nullptr)free(src);
		src = nullptr;
	}
}