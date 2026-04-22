#ifndef _XE_ALLOC_HPP_
#define _XE_ALLOC_HPP_

#define USE_MIMALLOC

#include "type/xeOrdinals.hpp"

//#include "mimalloc-new-delete.h"

namespace xe
{
	any_type_ptr_t xe_only_malloc(size_t byte_size) noexcept;

	any_type_ptr_t xe_only_realloc(any_type_ptr_t src, size_t byte_size) noexcept;

	any_type_ptr_t xe_olny_aligned_alloc(size_t alignment, size_t byte_size) noexcept;

	any_type_ptr_t xe_olny_aligned_realloc(any_type_ptr_t src, size_t alignment, size_t byte_size) noexcept;

	// malloc
	template<typename T> T* xe_malloc(size_t size) noexcept
	{
		return (T*)xe_only_malloc(sizeof(T) * size);
	}

	template<typename T> T* xe_realloc(any_type_ptr_t src, size_t size) noexcept
	{
		return (T*)xe_only_realloc(src, sizeof(T) * size);
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