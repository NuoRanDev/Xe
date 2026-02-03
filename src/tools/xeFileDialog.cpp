#include "Rfd.hpp"
#include "xeFileDialog.hpp"

namespace xe
{
	void FileDialog::set_directory(Path& dir) noexcept
	{
		u8_dir = dir;
	}
	
	void FileDialog::add_filter(const String& name, const dynamic_array<String>& extensions_list) noexcept
	{
		for (auto & extension: extensions_list)
		{
			extensions_filter_list.push_back(extension.data());
		}
		extensions_name = name.data();
	}

	Path FileDialog::save_file() noexcept
	{
		ref_str = rfd_save_file(u8_dir.path_str.data(), extensions_name, extensions_filter_list.data(), extensions_filter_list.size());
		return Path(xe::String(ref_str));
	}

	Path FileDialog::pick_file() noexcept
	{
		ref_str = rfd_pick_file(u8_dir.path_str.data(), extensions_name, extensions_filter_list.data(), extensions_filter_list.size());
		return Path(xe::String(ref_str));
	}

	Path FileDialog::pick_folder() noexcept
	{
		ref_str = rfd_pick_folder(u8_dir.path_str.data(), extensions_name, extensions_filter_list.data(), extensions_filter_list.size());
		return Path(xe::String(ref_str));
	}

	FileDialog::~FileDialog()
	{
		if (ref_str.data != nullptr)
		{
			free_rust_string(ref_str.data);
		}
	}
} // namespace xe is end