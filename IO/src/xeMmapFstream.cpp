#include "xeMmapFstream.h"

#if _WIN32
#include <Windows.h>
#endif // _WIN32

namespace xe
{
	void BasicMmapfstream::Release()
	{
#ifdef _WIN32
		if (pfile_start != nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping != nullptr)
			CloseHandle(hfile_mapping);
		if (c_dumpFileDescriptor != nullptr)
			CloseHandle(c_dumpFileDescriptor);
#endif // _WIN32 FUNCTION IS END
		pfile_start = nullptr;
		hfile_mapping = nullptr;
	}

	BasicMmapfstream::~BasicMmapfstream()
	{
		Release();
	}

	bool oMmapfstream::GetFilePtr(const SystemDefaultString* str)
	{
#ifdef _WIN32
		c_dumpFileDescriptor = CreateFile(
			str,
			GENERIC_READ,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (c_dumpFileDescriptor == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("Create file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return false;
		}
		hfile_mapping = CreateFileMapping(c_dumpFileDescriptor, NULL, PAGE_READONLY, 0, 0, NULL);
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

	bool iMmapfstream::GetFilePtr(const SystemDefaultString* str)
	{
#ifdef _WIN32
		c_dumpFileDescriptor = CreateFile(
			str,
			GENERIC_WRITE,
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
		hfile_mapping = CreateFileMapping(c_dumpFileDescriptor, NULL, PAGE_WRITECOPY, 0, 0, NULL);
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

	bool ioMmapfstream::GetFilePtr(const SystemDefaultString* str)
	{
#ifdef _WIN32
		c_dumpFileDescriptor = CreateFile(
			str,
			GENERIC_READ|GENERIC_WRITE,
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
}