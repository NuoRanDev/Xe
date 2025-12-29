#ifndef _XE_MEMORY_FILE_HPP_
#define _XE_MEMORY_FILE_HPP_

#include "type/xeOrdinals.hpp"
#include "string/xeString.hpp"

namespace xe
{
	class MemoryFile
	{
	public:
		MemoryFile() noexcept = default;

		MemoryFile(const MemoryFile&) noexcept = default;

		MemoryFile& operator=(const MemoryFile&) noexcept = default;

		MemoryFile& operator=(MemoryFile&&) noexcept = default;

		// load image file
		bool load_file_not_copy_in_memory(byte_t* mem_data, size_t data_size, String mem_file_name) noexcept;

		// get image data
		const byte_t* const get_file_data(size_t& out_data_size) const noexcept;

		// get offset data
		byte_t* get_write_offset_data(size_t offset) noexcept;

		// get file name
		String get_file_name() const noexcept { return file_name; }
		char const* c_file_name() const noexcept { return file_name.c_str(); }

		// release memory
		void release() noexcept;

		~MemoryFile() { release(); }

	protected:
		bool is_copy_in_memory;

		String file_name;

		byte_t* file_data;

		size_t file_data_size;
	};
} // namespace xe is end

#endif // _XE_MEMORY_FILE_HPP_ IS EOF