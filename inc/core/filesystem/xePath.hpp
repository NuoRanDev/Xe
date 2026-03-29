#ifndef _XE_PATH_HPP_
#define _XE_PATH_HPP_

#include "string/xeString.hpp"

#include <filesystem>

namespace xe
{

#if defined(_WIN32)
	using OS_STRING = U16StringRef;
#else
	using OS_STRING = U8StringRef;
#endif // defined(_WIN32) IS END

	namespace Path
	{
		String abs_path(String& path) noexcept;

		String base_name(String& path) noexcept;

		bool exists(const String& path)  noexcept;

		std::time_t get_create_time(const String& path)  noexcept;

		uint64_t get_size(const String& path)  noexcept;

		bool is_abs(const String& path)  noexcept;

		bool is_dir(const String& path)  noexcept;

		bool is_file(const String& path)  noexcept;

		std::pair<String, String> split(String& path)  noexcept;

		OS_STRING get_native_str(const String& path)  noexcept;
	} // namespace Path is end
} // namespace xe is end

#endif // !_XE_PATH_HPP_