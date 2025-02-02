#include "xeAlloc.hpp"

#if defined(USE_MIMALLOC)
#include <mimalloc.h>
#else
#include <cstdlib>
#endif // defined(USE_MIMALLOC) IS END

namespace xe
{
	void * xeCMallloc(size_t alloc_size)
	{
#if defined(USE_MIMALLOC)
		return mi_malloc(alloc_size);
#else
		return malloc(alloc_size);
#endif // defined(USE_MIMALLOC) IS END
	}

	void* xeCRealloc(void* src_ptr, size_t alloc_size)
	{
#if defined(USE_MIMALLOC)
		return mi_realloc(src_ptr, alloc_size);
#else
		return realloc(src_ptr, alloc_size);
#endif // defined(USE_MIMALLOC) IS END
	}
	void xeFree(void* src_ptr)
	{
#if defined(USE_MIMALLOC)
		mi_free(src_ptr);
#else
		mi_free(src_ptr);
#endif // defined(USE_MIMALLOC) IS END
	}
}