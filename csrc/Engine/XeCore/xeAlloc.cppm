export module xe.Core.xeAlloc;

import std;

#if defined(USE_MIMALLOC)
import "mimalloc.h";
#endif // USE_MIMALLOC

import xe.Core.xeOrdinals;

namespace xe
{
	export template<typename T> T* xeMalloc(xeSize alloc_number)
	{
#if defined(USE_MIMALLOC)
		return reinterpret_cast<T*>(mi_malloc(sizeof(T) * alloc_number));
#else
		return reinterpret_cast<T*>(std::malloc(sizeof(T) * alloc_number));
#endif // defined(USE_MIMALLOC) IS END
	}

	export template<typename T>T* xeRealloc(T* src_ptr, xeSize alloc_number)
	{
#if defined(USE_MIMALLOC)
		return reinterpret_cast<T*>(mi_realloc(src_ptr, sizeof(T) * alloc_number));
#else
		return reinterpret_cast<T*>(std::realloc(src_ptr, sizeof(T) * alloc_number));
#endif // defined(USE_MIMALLOC) IS END
	}

	export void xeFree(void* src_ptr)
	{
#if defined(USE_MIMALLOC)
		mi_free(src_ptr);
#else
		std::free(src_ptr);
#endif // defined(USE_MIMALLOC) IS END
	}

	export template<typename T> xeUint64 ARRAYSIZE(T* _ARR)
	{
		return ((xeUint64)(sizeof(_ARR) / sizeof(*(_ARR))));
	}

	export template<typename T> T* DeleteArray(T* input)
	{
		if (input != nullptr)
		{
			xeFree(input);
		}
		input = nullptr;
		return nullptr;
	}

	export template<typename T> T* DeleteSingla(T* input)
	{
		if (input != nullptr)
		{
			xeFree(input);
		}
		input = nullptr;
		return nullptr;
	}
} // namespace xe end