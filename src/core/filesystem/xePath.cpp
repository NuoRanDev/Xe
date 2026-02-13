#include "filesystem/xePath.hpp"
#include "memory/xeAlloc.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#endif

#if defined(_WIN32)
#else
#endif // OS

namespace xe
{
	namespace Path 
	{
		String abs_path(String& path) noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		String base_name(String& path) noexcept 
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		bool exists(String& path)  noexcept 
		{
#if defined(_WIN32)
			return static_cast<bool>(PathFileExistsW(get_native_str(path).data()));
#elif defined(__linux__)
#endif // OS
		}

		uint64_t get_create_time(String& path)  noexcept 
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		uint64_t get_size(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		bool is_abs(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		bool is_dir(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		bool is_file(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		std::pair<String, String> split(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
#endif // OS
		}

		OS_STRING get_native_str(const String& path)  noexcept
		{
#if defined(_WIN32)
			return U16StringRef(path);
#elif defined(__linux__)
			return path;
#endif // init windows native str
		}
	} // namespace Path is end
} // namespace xe is end