#ifndef _XE_MEMORY_FILE_H_
#define _XE_MEMORY_FILE_H_

#include "type/xeOrdinals.h"
#include "string/xeString.h"

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
		bool load_file_not_copy_in_memory(byte_t* mem_data, xeString mem_file_name) noexcept;

		// get image data
		const byte_t* const get_file_data(size_t& out_data_size) const noexcept;

		// release memory
		void release();

		~MemoryFile() { release(); }

	protected:
		bool is_copy_in_memory;

		xeString file_name;

		byte_t* file_data;
	};
} // namespace xe is end

#endif // _XE_MEMORY_FILE_H_ IS EOF