#ifndef _INC_MMAP_F_STREAM_H_
#define _INC_MMAP_F_STREAM_H_

#include "xeCoreConfig.h"

namespace xe
{
	class XE_CORE_PRIVATE_API oMmapfstream
	{
	public:

		oMmapfstream() = default;
		
		bool GetFilePtr(char* str);
		
		bool FstraemByteMemcpy(byte_t* dst, size_t start, size_t offset);
		
		template <typename T> T* GetFstreamPtr(size_t start);
		
		template <typename T> bool FstraemStartMemcpy(T* dst, size_t number);
		
		template <typename T> bool FstraemMemcpy(T* dst, size_t start, size_t number);
		
		void Release();
		
		~oMmapfstream();
	private:
		size_t file_size;

		void* pfile_start;
#ifdef _WIN32
		void* hfile_mapping;
#endif // _WIN32
	};

	class iMmapfstream
	{
	};
}

#endif // !_INC_MMAP_F_STREAM_H_