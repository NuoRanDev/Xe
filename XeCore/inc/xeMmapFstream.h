#ifndef _INC_MMAP_F_STREAM_H_
#define _INC_MMAP_F_STREAM_H_

#include "xeCoreConfig.h"
#include "xeCoreClrOutput.h"

namespace xe
{
	class XE_CORE_PRIVATE_API oMmapfstream
	{
	public:

		oMmapfstream() = default;
		
		bool GetFilePtr(const char* str);
		
		bool FstraemByteMemcpy(byte_t* dst, size_t start, size_t offset);
		
		template<typename T> T* GetFstreamPtr(size_t offset_byte)
		{
			if (offset_byte > file_size)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (T*)((byte_t*)pfile_start + offset_byte);
		}
		
		template<typename T> bool FstraemStartMemcpy(T* dst, size_t number)
		{
			if (number * sizeof(T) > file_size)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			memcpy(dst, pfile_start, sizeof(T) * number);
			return true;
		}
		
		template<typename T> bool FstraemMemcpy(T* dst, size_t start, size_t number)
		{
			if (file_size > (start + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			memcpy(dst, (byte_t*)pfile_start + start, number * sizeof(T));
			return true;
		}
		
		void Release();
		
		~oMmapfstream();
	private:
		size_t file_size;

		void* pfile_start;
#ifdef _WIN32
		void* hfile_mapping;
		void* c_dumpFileDescriptor;
#endif // _WIN32
	};

	class iMmapfstream
	{
	};
}

#endif // !_INC_MMAP_F_STREAM_H_