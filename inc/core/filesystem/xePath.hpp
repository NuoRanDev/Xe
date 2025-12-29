#ifndef _XE_PATH_HPP_
#define _XE_PATH_HPP_

#include "string/xeString.hpp"

#include <vector>
#include <filesystem>

#if defined(_WIN32)
using OS_STRING = wchar_t;
#else
using OS_STRING = utf8_t;
#endif // defined(_WIN32) IS END


namespace xe
{
	class Path final
	{
	public:
		String path_str;

		Path() noexcept;

		Path& operator=(const Path& path) noexcept;

		Path(const String& path) noexcept;

		Path& operator=(const String& path) noexcept;

		Path abs_path() noexcept;

		Path base_name() noexcept;

		bool exists() const noexcept;

		uint64_t get_create_time() const noexcept;

		bool get_size() const noexcept;

		bool is_abs() const noexcept;

		bool is_dir() const noexcept;

		bool is_file() const noexcept;

		std::pair<Path, Path> split() const noexcept;

		const OS_STRING* get_native_str() const noexcept;

		~Path();
	private:
#if defined(_WIN32)

		void init_native_str();

		OS_STRING* native_str;
#endif // windows string is utf16
	};
} // namespace xe is end

#endif // !_XE_PATH_HPP_