#ifndef _XE_PATH_H_
#define _XE_PATH_H_

#include "string/xeString.h"

#include <vector>
#include <filesystem>

namespace xe
{
	class Path :std::filesystem::path
	{
	public:
		Path()								= default;

		Path& operator=(const Path&)		= default;
		
		Path& operator=(Path&&) noexcept	= default;

		Path(const xeString _path_str)
		{
			path(_path_str.data());
		}

		// laod xstring in std path
		Path& operator=(const xeString _path_str)
		{
			Path(_path_str);
			return *this;
		}

		xeString xeu8string()
		{
			auto str = this->u8string();
			return xeString(str.c_str(), str.size());
		}

		~Path(){}
	};
} // namespace xe is end

#endif // !_XE_PATH_H_