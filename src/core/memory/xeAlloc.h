#ifndef _XE_ALLOC_H_
#define _XE_ALLOC_H_

#include <cstdlib>

namespace xe
{
	// malloc
	template<typename T> T* xe_malloc(size_t size) noexcept
	{
		return (T*)malloc(sizeof(T) * size);
	}

	template<typename T> T* xe_realloc(T* src, size_t size) noexcept
	{
		return (T*)realloc(src, sizeof(T) * size);
	}

	void xe_free(void* src) noexcept;

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

#endif // _XE_ALLOC_H_ IS EOF