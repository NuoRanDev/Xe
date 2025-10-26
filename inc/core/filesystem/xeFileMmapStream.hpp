#ifndef _INC_XE_FILE_MMAP_STREAM_HPP_
#define _INC_XE_FILE_MMAP_STREAM_HPP_

#include "type/xeOrdinals.hpp"
#include "string/xeString.hpp"
#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

#include <format>

namespace xe
{
	class Mmapfstream
	{
	public:

		Mmapfstream() noexcept = default;

		Mmapfstream(const Mmapfstream&) noexcept = default;

		Mmapfstream& operator=(const Mmapfstream&) noexcept = default;

		Mmapfstream& operator=(Mmapfstream&&) noexcept = default;

		// dircetly get file data pointer

		const byte_t* get_mmap_ptr() const noexcept { return (const byte_t*)pfile_start; }

		const byte_t* get_mmap_offset_ptr(size_t offset_byte) const noexcept
		{
			if (file_size < offset_byte)
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
					"xeCore",
					std::format("Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (byte_t*)pfile_start + offset_byte;
		}

		bool get_file_size(const utf8_t* path) noexcept;

		// read file data to dst (copy)

		template<typename T> bool read(T* dst, size_t number) const noexcept { return read(0, dst, number); }

		template<typename T> bool read(size_t offset_byte, T* dst, size_t number) const noexcept
		{
			if (file_size < (offset_byte + number * sizeof(T)))
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
					"xeCore",
					std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			std::memcpy(dst, (byte_t*)pfile_start + offset_byte, number * sizeof(T));
			return true;
		}

		bool open_file(const utf8_t* path) noexcept;

		void release() noexcept;

		// not safe function

		byte_t* data(uint64_t &size) noexcept 
		{
			size = file_size;
			return (byte_t*)pfile_start; 
		}

#if !defined(EXPORT_C_SHARP_API)
		bool open_file(xeString file_path)
		{
			return open_file(file_path.data());
		}
#endif // defined(EXPORT_C_SHARP_API)

	protected:
		uint64_t file_size;

		void* pfile_start;

#if defined(_WIN32)
		void* hfile_mapping;
		void* c_dump_file_descriptor;
#elif defied(__linux__)
#else
#error "SUPPORTED ERROR: NOT SUPPORT THIS SYSTEM!"
#endif // SYSTEM CHOOSE
	};
} // namespace xe is end

#endif // _INC_XE_FILE_MMAP_STREAM_HPP_ IS EOF