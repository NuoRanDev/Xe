#include "file/image/xeImgaeFile.hpp"

#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

namespace xe
{
	bool MemoryFile::load_file_not_copy_in_memory(byte_t* mem_data, size_t data_size, String& mem_file_name) noexcept
	{
		if (mem_data == nullptr)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Memory file data is empty");
			return false;
		}
		file_data = mem_data;
		file_name = mem_file_name;
		file_data_size = data_size;
		is_copy_in_memory = false;
		return true;
	}

	const byte_t* const MemoryFile::get_file_data(size_t& out_data_size) const noexcept
	{
		if (file_data == nullptr)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Memory file data is empty");
			out_data_size = 0;
			return nullptr;
		}
		out_data_size = file_data_size;
		return file_data;
	}

	void MemoryFile::release() noexcept
	{
		if (is_copy_in_memory && file_data != nullptr)
		{
			xe_free(file_data);
		}
		file_data = nullptr;
		is_copy_in_memory = false;
	}
} // namespace xe is end