#ifndef _INC_MMAP_F_STREAM_HPP_
#define _INC_MMAP_F_STREAM_HPP_

#include "xeIOConfing.hpp"

namespace xe
{
	class XE_IO_EXPORT_C_PLUS_PLUS_API BasicMmapfstream
	{
	public:
		BasicMmapfstream();

		template<typename T> T* GetFstreamPtr(size_t offset_byte)
		{
			if (file_size < offset_byte)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (T*)((byte_t*)pfile_start + offset_byte);
		}
				
		void Release();
		
		~BasicMmapfstream();
		
		uint64_t file_size;

	protected:
		void* pfile_start;
#if defined(_WIN32)
		void* hfile_mapping;
		void* c_dumpFileDescriptor;
#elif defied(__linux__)
#else
#error "SUPPORTED ERROR: NOT SUPPORT THIS SYSTEM!"
#endif // _WIN32
		void GetFileSize(const char* file_name);
#if !defined(XE_FILE_IO_WRITE_ONLY)
		std::mutex* io_lock;
#endif // READ OLNLY LOCK IS END
	};

#ifndef XE_FILE_IO_WRITE_ONLY
	class XE_IO_EXPORT_C_PLUS_PLUS_API oMmapfstream :virtual public BasicMmapfstream
	{
	public:
		virtual bool GetFilePtr(const char* str);

		template<typename T> bool FstraemStartMemcpyOut(T* dst, size_t number)
		{
			if (file_size < number * sizeof(T))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
#if !defined( XE_FILE_IO_WRITE_ONLY)
			io_lock->lock();
#endif // READ OLNLY LOCK IS END
			memcpy(dst, pfile_start, sizeof(T) * number);
#if !defined( XE_FILE_IO_WRITE_ONLY)
			io_lock->unlock();
#endif // READ OLNLY LOCK IS END
			return true;
		}

		template<typename T> bool FstraemMemcpyOut(T* dst, size_t start, size_t number)
		{
			if (file_size < (start + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
#if !defined( XE_FILE_IO_WRITE_ONLY)
			io_lock->lock();
#endif // READ OLNLY LOCK IS END
			memcpy(dst, (byte_t*)pfile_start + start, number * sizeof(T));
#if !defined( XE_FILE_IO_WRITE_ONLY)
			io_lock->unlock();
#endif // READ OLNLY LOCK IS END
			return true;
		}
	};
#endif // XE_FILE_IO_READ_ONLY IS END

#ifndef XE_FILE_IO_READ_ONLY
	class XE_IO_EXPORT_C_PLUS_PLUS_API iMmapfstream :virtual public BasicMmapfstream
	{
	public:
		virtual bool GetFilePtr(const char* str);

		template<typename T> void FstraemStartMemcpyIn(T* dst, size_t number)
		{
			io_lock->lock();
			memcpy(pfile_start, dst, sizeof(T) * number);
			io_lock->unlock();
		}

		template<typename T> bool FstraemMemcpyIn(T* dst, size_t start, size_t number)
		{
			if (file_size < (start + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			io_lock->lock();
			memcpy((byte_t*)pfile_start + start, dst, number * sizeof(T));
			io_lock->unlock();
			return true;
		}
	};
#endif // XE_FILE_IO_WRITE_ONLY IS END

#if !defined(XE_FILE_IO_WRITE_ONLY) && !defined(XE_FILE_IO_READ_ONLY)
	class XE_IO_EXPORT_C_PLUS_PLUS_API ioMmapfstream :virtual public BasicMmapfstream, public iMmapfstream, public oMmapfstream
	{
	public:
		virtual bool GetFilePtr(const char* str);
	};
#endif // DEFAULT FILE IO IS END
}

#endif // !_INC_MMAP_F_STREAM_HPP_