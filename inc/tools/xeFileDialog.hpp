#ifndef _INC_XE_RFD_HPP_
#define _INC_XE_RFD_HPP_

#include "filesystem/xePath.hpp"

#include "type/xeDataStruction.hpp"

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

		void set_directory(String& dir) noexcept;

		void add_filter(const String& name, const dynamic_array<String>& extensions_list) noexcept;

		String save_file() noexcept;

		String pick_folder() noexcept;

		String pick_file() noexcept;

		~FileDialog();

	private:

		dynamic_array<const utf8_t*> extensions_filter_list;
		const utf8_t* extensions_name;

		String u8_dir;

		RustString ref_str;
	};
} // namespace xe is end

#endif // _INC_XE_RFD_HPP_ IS EOF