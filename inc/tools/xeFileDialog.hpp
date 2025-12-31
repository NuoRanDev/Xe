#ifndef _INC_XE_RFD_HPP_
#define _INC_XE_RFD_HPP_

#include "filesystem/xePath.hpp"

namespace xe
{
	class FileDialog
	{
	public:

		FileDialog() noexcept
		{
			extensions_name = nullptr;
			ref_str =
			{
				.size = 0,
				.data = nullptr,
			};
		}

		void set_directory(Path& dir) noexcept;

		void add_filter(const String& name, const std::vector<String>& extensions_list) noexcept;

		Path save_file() noexcept;

		Path pick_folder() noexcept;

		Path pick_file() noexcept;

		~FileDialog();

	private:

		std::vector<const utf8_t*> extensions_filter_list;
		const utf8_t* extensions_name;

		Path u8_dir;

		RustString ref_str;
	};
} // namespace xe is end

#endif // _INC_XE_RFD_HPP_ IS EOF