#include "xeMmapFstream.h"

#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace xe
{
	bool oMmapfstream::GetFilePtr(const char* str)
	{
#ifdef _WIN32
		c_dumpFileDescriptor = CreateFileA(
			str,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (c_dumpFileDescriptor == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Create file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return false;
		}
		hfile_mapping = CreateFileMapping(c_dumpFileDescriptor, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (hfile_mapping == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Open file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return false;
		}
		pfile_start = MapViewOfFile(hfile_mapping, FILE_MAP_READ, 0, 0, 0);
		if (pfile_start == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Map file mapping failed ,SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			CloseHandle(hfile_mapping);
			return false;
		}
#endif // _WIN32 FUNCTION IS END
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
#ifdef _WIN32
		if (pfile_start == nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping == nullptr)
			CloseHandle(hfile_mapping);
		if (c_dumpFileDescriptor == nullptr)
			CloseHandle(c_dumpFileDescriptor);
#endif // _WIN32 FUNCTION IS END
		pfile_start = nullptr;
		hfile_mapping = nullptr;
	}

	oMmapfstream::~oMmapfstream()
	{
		Release();
	}
}