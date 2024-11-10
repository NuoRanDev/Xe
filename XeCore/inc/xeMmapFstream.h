#ifndef _INC_MMAP_F_STREAM_H_
#define _INC_MMAP_F_STREAM_H_

#include "xeCoreConfig.h"

namespace xe
{
	XE_PRIVATE_API class iMmapfstream;
	XE_PRIVATE_API class basicMmapfstream;

	class iMmapfstream
	{
	public:

		iMmapfstream() = default;
		
		bool GetFilePtr(char* str);
		
		bool FstraemByteMemcpy(byte_t* dst, size_t start, size_t offset);
		
		template <typename T> T* GetFstreamPtr(size_t start);
		
		template <typename T> bool FstraemStartMemcpy(T* dst, size_t number);
		
		template <typename T> bool FstraemMemcpy(T* dst, size_t start, size_t number);
		
		void Release();
		
		~iMmapfstream();
	private:
		size_t file_size;

		void* pfile_start;
#ifdef _WIN32
		void* hfile_mapping;
#endif // _WIN32
	};

	class oMmapfstream
	{
	};
}

#endif // !_INC_MMAP_F_STREAM_H_