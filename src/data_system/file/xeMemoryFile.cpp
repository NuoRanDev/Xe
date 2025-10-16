#include "file/image/xeImgaeFile.hpp"

#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

namespace xe
{
	bool MemoryFile::load_file_not_copy_in_memory(byte_t* mem_data, xeString mem_file_name) noexcept
	{
		if (mem_data == nullptr);
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Image file data is empty");
			return false;
		}
		file_data = mem_data;
		file_name = mem_file_name;
		is_copy_in_memory = false;
		return true;
	}

	const byte_t* const MemoryFile::get_file_data(size_t& out_data_size) const noexcept
	{
		if (file_data == nullptr)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Image file data is empty");
			return nullptr;
		}
		out_data_size = 0;
		return file_data;
	}

	void MemoryFile::release()
	{
		if (is_copy_in_memory and file_data != nullptr)
		{
			xe_free(file_data);
		}
		file_data = nullptr;
		file_name.release();
		is_copy_in_memory = false;
	}
} // namespace xe is end