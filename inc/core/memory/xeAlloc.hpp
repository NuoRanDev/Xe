#ifndef _XE_ALLOC_HPP_
#define _XE_ALLOC_HPP_

#include <cstdlib>

#include "type/xeOrdinals.hpp"
#include "mimalloc-override.h"
//#include "mimalloc-new-delete.h"

namespace xe
{
	any_type_ptr_t xe_only_malloc(size_t size) noexcept;

	// malloc
	template<typename T> T* xe_malloc(size_t size) noexcept
	{
		return (T*)xe_only_malloc(sizeof(T) * size);
	}

	template<typename T> T* xe_realloc(T* src, size_t size) noexcept
	{
		return (T*)mi_realloc(src, sizeof(T) * size);
	}

	any_type_ptr_t xe_free(any_type_ptr_t src) noexcept;

	template<typename T> void xe_delete_array(T* src) noexcept
	{
		if (src != nullptr)
		{
			delete[] src;
			src = nullptr;
		}
	}

	template<typename T> void xe_delete(T* src) noexcept
	{
		if (src != nullptr)
		{
			delete src;
			src = nullptr;
		}
	}
} // namespace xe is end

#endif // _XE_ALLOC_HPP_ IS EOF