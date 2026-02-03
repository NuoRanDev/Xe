#include "memory/xeAlloc.hpp"

namespace xe
{
	any_type_ptr_t xe_only_malloc(size_t size) noexcept
	{
		return mi_malloc(size);
	}

	any_type_ptr_t xe_free(any_type_ptr_t src) noexcept
	{
		if (src != nullptr)mi_free(src);
		return nullptr;
	}
}