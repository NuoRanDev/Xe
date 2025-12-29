#include "filesystem/xePath.hpp"
#include "memory/xeAlloc.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#endif

#if defined(_WIN32)
#else
#endif // OS

#include <map>

namespace xe
{
	Path::Path() noexcept
	{
		native_str = nullptr;
	}

	Path& Path::operator = (const Path& path) noexcept
	{
		this->path_str = path.path_str;
		return *this;
	}

	Path::Path(const String& path) noexcept
	{
		path_str = path;
#if defined(_WIN32)
		if (!(path.is_empty()))
			init_native_str();
		else
		{
			native_str = nullptr;
		}
#endif
	}

	Path& Path::operator=(const String& path) noexcept
	{
		this->path_str = path;
#if defined(_WIN32)
		if (!(path.is_empty()))
			init_native_str();
		else
		{
			native_str = nullptr;
		}
#endif
		return *this;
	}

	Path Path::abs_path() noexcept
	{
		return Path();
	}

	Path Path::base_name() noexcept
	{
		return Path();
	}

	bool Path::exists() const noexcept
	{
#if defined(_WIN32)
		return static_cast<bool>(PathFileExistsW(native_str));
#else
#endif // OS
	}

	uint64_t Path::get_create_time() const noexcept
	{
		return 0;
	}

	bool Path::get_size() const noexcept
	{
		return false;
	}

	bool Path::is_abs() const noexcept
	{
		return false;
	}

	bool Path::is_dir() const noexcept
	{
		return false;
	}

	bool Path::is_file() const noexcept
	{
		return false;
	}

	std::pair<Path, Path> Path::split() const noexcept
	{
		return std::pair<Path, Path>();
	}

	const OS_STRING* Path::get_native_str() const noexcept
	{
#if defined(_WIN32)
		return native_str;
#else
		return path_str.data();
#endif // get system default string format is end
	}

	Path::~Path()
	{
#if defined(_WIN32)
		xe_free(native_str);
		native_str = nullptr;
#endif // delete windows native str
	}

#if defined(_WIN32)
	void Path::init_native_str()
	{
		int32_t u16_size = MultiByteToWideChar(CP_UTF8, 0, path_str.c_str(), -1, nullptr, 0);
		if (native_str == nullptr)
		{
			native_str = xe_malloc<OS_STRING>(u16_size);
		}
		else
		{
			native_str = xe_realloc<OS_STRING>(native_str, u16_size);
		}
		MultiByteToWideChar(CP_UTF8, 0, path_str.c_str(), -1, native_str, u16_size);
	}
#endif  // init windows native str
} // namespace xe is end