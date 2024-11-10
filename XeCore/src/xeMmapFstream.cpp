#include "xeMmapFstream.h"

#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace xe
{
	bool iMmapfstream::GetFilePtr(char* str)
	{
#ifdef _WIN32
		hfile_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, L"ShareFile");
		if (hfile_mapping == nullptr)
		{
#ifdef _DEBUG
			std::cout << std::format("Open file mapping failed , ERROR CODE:{0}\n", GetLastError());
#endif // DEBUG IS END
			return false;
		}
#endif // _WIN32
		pfile_start = MapViewOfFile(hfile_mapping, FILE_MAP_READ, 0, 0, 0);
		if (pfile_start == nullptr)
		{
#ifdef _DEBUG
			std::cout << std::format("Map file mapping failed , ERROR CODE:{0}\n", GetLastError());
#endif // DEBUG IS END
			CloseHandle(hfile_mapping);
			return -1;
		}
		return true;
	}

	bool iMmapfstream::FstraemByteMemcpy(byte_t* dst, size_t start, size_t offset)
	{
		if (file_size > start + offset)
		{
			return false;
		}
		memcpy(dst, (byte_t*)pfile_start + start, offset);
		return true;
	}

	void iMmapfstream::Release()
	{
		if (pfile_start == nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping == nullptr)
			CloseHandle(hfile_mapping);
		pfile_start = nullptr;
		hfile_mapping = nullptr;
	}

	iMmapfstream::~iMmapfstream()
	{
		Release();
	}

	template<typename T> inline T* xe::iMmapfstream::GetFstreamPtr(size_t start)
	{
		if (start > file_size)
		{
#ifdef _DEBUG
			std::cout << std::format("Out of memry ERROR :start {0}, file size {1}\n", start, file_size);
#endif // DEBUG IS END
			return nullptr;
		}
		return pfile_start + file_size;
	}

	template<typename T> bool iMmapfstream::FstraemStartMemcpy(T* dst, size_t number)
	{
		if (start > file_size)
		{
#ifdef _DEBUG
			std::cout << std::format("Out of memry ERROR :start {0}, file size {1}\n", sizeof(T) * number, file_size);
#endif // DEBUG IS END
			return false;
		}
		memcpy(dst, pfile_start, sizeof(T) * number);
		return true;
	}

	template<typename T> bool iMmapfstream::FstraemMemcpy(T* dst, size_t start, size_t number)
	{
		if (file_size > (start + number * sizeof(T)))
		{
#ifdef _DEBUG
			std::cout << std::format("Out of memry ERROR :start {0}, file size {1}\n", sizeof(T) * number, file_size);
#endif // DEBUG IS END
			return false;
		}
		memcpy(dst, (byte_t*)pfile_start + start, number * sizeof(T));
		return true;
	}
}