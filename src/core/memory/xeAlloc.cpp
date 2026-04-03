#include "memory/xeAlloc.hpp"

#if defined(USE_MIMALLOC)
#include "mimalloc-override.h"
#else
#include <cstdlib>
#endif // USE_MIMALLOC

namespace xe
{
	any_type_ptr_t xe_only_malloc(size_t byte_size) noexcept
	{
		return malloc(byte_size);
	}

	any_type_ptr_t xe_only_realloc(any_type_ptr_t src, size_t byte_size) noexcept
	{
		return realloc(src, byte_size);
	}

	any_type_ptr_t xe_free(any_type_ptr_t src) noexcept
	{
		if (src != nullptr)free(src);
		return nullptr;
	}
}