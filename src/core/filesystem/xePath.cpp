#include "filesystem/xePath.hpp"
#include "memory/xeAlloc.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/stat.h> 
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
			return String(realpath(path.c_str(), nullptr));
#endif // OS
		}

		String base_name(String& path) noexcept 
		{
#if defined(_WIN32)
#elif defined(__linux__)
			return String(basename(path.c_str()));
#endif // OS
		}

		bool exists(String& path)  noexcept 
		{
#if defined(_WIN32)
			return static_cast<bool>(PathFileExistsW(get_native_str(path).data()));
#elif defined(__linux__)
			return access(path.c_str(), F_OK) == 0;	
#endif // OS
		}

		uint64_t get_create_time(String& path)  noexcept 
		{
#if defined(_WIN32)
#elif defined(__linux__)
			struct stat file_stat;
			if (stat(path.c_str(), &file_stat) != 0)
			{
				return 0;
			}
			return static_cast<uint64_t>(file_stat.st_ctime);
#endif // OS
		}

		uint64_t get_size(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
			struct stat file_stat;
			if (stat(path.c_str(), &file_stat) != 0)
			{
				return 0;
			}
			return static_cast<uint64_t>(file_stat.st_size);
#endif // OS
		}

		bool is_abs(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
			return path.data()[0] == '/';
#endif // OS
		}

		bool is_dir(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
			struct stat file_stat;
			if (stat(path.c_str(), &file_stat) != 0)
			{
				return false;
			}
			return S_ISDIR(file_stat.st_mode);
#endif // OS
		}

		bool is_file(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
			struct stat file_stat;
			if (stat(path.c_str(), &file_stat) != 0)
			{
				return false;
			}
			return S_ISREG(file_stat.st_mode);
#endif // OS
		}

		std::pair<String, String> split(String& path)  noexcept
		{
#if defined(_WIN32)
#elif defined(__linux__)
			if(!exists(path)) return {String(), String()};
			if(is_dir(path)) return {path, String()};
			else {
				String base_name_str = base_name(path);
				String dir_name_str = path.slice(0, path.get_characters_number() - base_name_str.get_characters_number());
				return {dir_name_str, base_name_str};
			}

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