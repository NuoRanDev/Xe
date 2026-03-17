#include "filesystem/xePath.hpp"
#include "memory/xeAlloc.hpp"
#include "log/xeLogOutput.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#include <wchar.h>
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
			String out;
			auto w_path = Path::get_native_str(path);
			PathStripPathW(w_path.get_win32_str_pdata());
			w_path.ptr_resize(std::wcslen(w_path.data()) + 1);
			w_path.to_utf8(out);
			return out;
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

		std::time_t get_create_time(String& path)  noexcept
		{
#if defined(_WIN32)
			FILETIME create_time;
			ULARGE_INTEGER ui;
			WIN32_FILE_ATTRIBUTE_DATA wfad;

			if(!exists(path))
			{
				XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeFilesystem",
					"File/Dir is not existsed");
				return -1;
			}
			if (GetFileAttributesExW(Path::get_native_str(path).data(), GetFileExInfoStandard, &wfad))
			{
				create_time = wfad.ftCreationTime;
				ui.LowPart = create_time.dwLowDateTime;
				ui.HighPart = create_time.dwHighDateTime;
				return ((std::time_t)(ui.QuadPart - 116444736000000000) / 10000000);
			}
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeFilesystem",
				std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return -1;
#elif defined(__linux__)
			struct stat file_stat;
			if (stat(path.c_str(), &file_stat) != 0)
			{
				return 0;
			}
			return file_stat.st_ctime;
#endif // OS
		}

		uint64_t get_size(String& path)  noexcept
		{
#if defined(_WIN32)
			LARGE_INTEGER info;
			WIN32_FILE_ATTRIBUTE_DATA wfad;

			if (!exists(path))
			{
				XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeFilesystem",
					"File/Dir is not existsed");
				return 0;
			}
			if (GetFileAttributesExW(Path::get_native_str(path).data(), GetFileExInfoStandard, &wfad))
			{
				info.HighPart = wfad.nFileSizeHigh;
				info.LowPart = wfad.nFileSizeLow;
				return static_cast<uint64_t>(info.QuadPart);
			}
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeFilesystem",
				std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return 0;
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
			return ('A' <= path.data()[0] && path.data()[0] <= 'Z');
#elif defined(__linux__)
			return path.data()[0] == '/';
#endif // OS
		}

		bool is_dir(String& path)  noexcept
		{
#if defined(_WIN32)
			DWORD dwat = GetFileAttributesW(Path::get_native_str(path).data());
			if (dwat == INVALID_FILE_ATTRIBUTES)
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeFilesystem", "Path is not exist");
			}
			return dwat & FILE_ATTRIBUTE_DIRECTORY;
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
			return !(is_dir(path));
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
			//PathUnquoteSpacesW获取路径
#elif defined(__linux__)
			if (!exists(path)) return { String(), String() };
			if (is_dir(path)) return { path, String() };
			else 
			{
				String base_name_str = base_name(path);
				String dir_name_str = path.slice(0, path.get_characters_number() - base_name_str.get_characters_number());
				return { dir_name_str, base_name_str };
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