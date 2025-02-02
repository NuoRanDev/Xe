#ifndef _INC_XE_ALLOC_HPP_
#define _INC_XE_ALLOC_HPP_

#define USE_MIMALLOC

#include "xeCoreConfig.hpp"

namespace xe
{
	// malloc
	EXTERN_C_STMT XE_CORE_EXPORT_C_PLUS_PLUS_API void* xeCMallloc(size_t alloc_size);
	template<typename T> T* xeMallloc(size_t alloc_number)
	{
		return (T*)xeCMallloc(sizeof(T) * alloc_number);
	}
	// realloc
	EXTERN_C_STMT XE_CORE_EXPORT_C_PLUS_PLUS_API void* xeCRealloc(void* src_ptr, size_t alloc_size);
	template<typename T>T* xeRealloc(void* src_ptr, size_t alloc_number)
	{
		return (T*)xeCRealloc(src_ptr, sizeof(T) * alloc_number);
	}
	// free
	EXTERN_C_STMT XE_CORE_EXPORT_C_PLUS_PLUS_API void xeFree(void* src_ptr);
}

#endif // _INC_XE_ALLOC_HPP_ IS EOF