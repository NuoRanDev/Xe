#ifndef _INC_MMAP_F_STREAM_H_
#define _INC_MMAP_F_STREAM_H_

#include "xeCore.hpp"
#include "xeCoreClrOutput.h"

namespace xe
{
	class XE_EXPORT_C_PLUS_PLUS_API BasicMmapfstream
	{
	public:
		BasicMmapfstream() = default;

		template<typename T> T* GetFstreamPtr(size_t offset_byte)
		{
			if (offset_byte > file_size)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (T*)((byte_t*)pfile_start + offset_byte);
		}
				
		void Release();
		
		~BasicMmapfstream();
		
		size_t file_size;

	protected:
		void* pfile_start;
#if defined(_WIN32)
		void* hfile_mapping;
		void* c_dumpFileDescriptor;
#elif defied(__linux__)
#else
#error "SUPPORTED ERROR: NOT SUPPORT THIS SYSTEM!"
#endif // _WIN32
	};

#ifndef XE_FILE_IO_WRITE_ONLY
	class XE_EXPORT_C_PLUS_PLUS_API oMmapfstream :virtual public BasicMmapfstream
	{
	public:
		virtual bool GetFilePtr(const SystemDefaultString* str);

		template<typename T> bool FstraemStartMemcpyOut(T* dst, size_t number)
		{
			if (number * sizeof(T) > file_size)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			memcpy(dst, pfile_start, sizeof(T) * number);
			return true;
		}

		template<typename T> bool FstraemMemcpyOut(T* dst, size_t start, size_t number)
		{
			if (file_size > (start + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			memcpy(dst, (byte_t*)pfile_start + start, number * sizeof(T));
			return true;
		}
	};
#endif // XE_FILE_IO_READ_ONLY IS END

#ifndef XE_FILE_IO_READ_ONLY
	class XE_EXPORT_C_PLUS_PLUS_API iMmapfstream :virtual public BasicMmapfstream
	{
	public:
		virtual bool GetFilePtr(const SystemDefaultString* str);

		template<typename T> void FstraemStartMemcpyIn(T* dst, size_t number)
		{
			memcpy(pfile_start, dst, sizeof(T) * number);
		}

		template<typename T> bool FstraemMemcpyOut(T* dst, size_t start, size_t number)
		{
			if (file_size > (start + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			memcpy((byte_t*)pfile_start + start, dst, number * sizeof(T));
			return true;
		}
	};
#endif // XE_FILE_IO_WRITE_ONLY IS END

#if !defined(XE_FILE_IO_WRITE_ONLY) && !defined(XE_FILE_IO_READ_ONLY)
	class XE_EXPORT_C_PLUS_PLUS_API ioMmapfstream :virtual public BasicMmapfstream, public iMmapfstream, public oMmapfstream
	{
	public:
		virtual bool GetFilePtr(const SystemDefaultString* str);
	};
#endif // DEFAULT FILE IO IS END
}

#ifdef EXPORT_C_SHARP_API

#endif // EXPORT_C_SHARP_API IS END


#endif // !_INC_MMAP_F_STREAM_H_