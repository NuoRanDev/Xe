#include "xeMmapFstream.h"
#include "XeCoreClrOutput.h"
#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace xe
{
	bool oMmapfstream::GetFilePtr(char* str)
	{
#ifdef _WIN32
		hfile_mapping = OpenFileMapping(FILE_MAP_READ, FALSE, L"ShareFile");
		if (hfile_mapping == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Open file mapping failed , SYSTEM ERROR CODE:{0}\n", GetLastError()).c_str());
			return false;
		}
#endif // _WIN32
		pfile_start = MapViewOfFile(hfile_mapping, FILE_MAP_READ, 0, 0, 0);
		if (pfile_start == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Map file mapping failed ,SYSTEM ERROR CODE:{0}\n", GetLastError()).c_str());
			CloseHandle(hfile_mapping);
			return false;
		}
		return true;
	}

	bool oMmapfstream::FstraemByteMemcpy(byte_t* dst, size_t start, size_t offset)
	{
		if (file_size > start + offset)
		{
			return false;
		}
		memcpy(dst, (byte_t*)pfile_start + start, offset);
		return true;
	}

	void oMmapfstream::Release()
	{
		if (pfile_start == nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping == nullptr)
			CloseHandle(hfile_mapping);
		pfile_start = nullptr;
		hfile_mapping = nullptr;
	}

	oMmapfstream::~oMmapfstream()
	{
		Release();
	}

	template<typename T> T* oMmapfstream::GetFstreamPtr(size_t offset_byte)
	{
		if (offset_byte > file_size)
		{
			XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}\n", offset_byte, file_size).c_str());
			return nullptr;
		}
		return (T*)((byte_t*)pfile_start + offset_byte);
	}

	template<typename T> bool oMmapfstream::FstraemStartMemcpy(T* dst, size_t number)
	{
		if (number * sizeof(T) > file_size)
		{
			XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}\n", sizeof(T) * number, file_size).c_str());
			return false;
		}
		memcpy(dst, pfile_start, sizeof(T) * number);
		return true;
	}

	template<typename T> bool oMmapfstream::FstraemMemcpy(T* dst, size_t start, size_t number)
	{
		if (file_size > (start + number * sizeof(T)))
		{
			XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}\n", sizeof(T) * number, file_size).c_str());
			return false;
		}
		memcpy(dst, (byte_t*)pfile_start + start, number * sizeof(T));
		return true;
	}
}