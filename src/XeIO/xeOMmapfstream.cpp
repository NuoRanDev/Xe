import xe.xeIO.xeOMmapfstream;

import std;
import xe.xeCore.xeOrdinals;
import xe.xeCore.xeCoreClrOutput;

#if defined(_WIN32)
#include <Windows.h>
#endif // OS inc

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
		file_size = 0;
	}

	bool BasicMmapfstream::GetFileSize(const char* file_name)
	{
		if (!std::filesystem::exists(file_name))
		{
			XE_WARNING_OUTPUT("CAN'T FIND FILE");
			return false;
		}
		file_size = std::filesystem::file_size(file_name);
		return true;
	}

	bool oMmapfstream::GetFilePtr(const char* str)
	{
		if (!GetFileSize(str))
			return false;
#ifdef _WIN32
		c_dumpFileDescriptor = CreateFileA(
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
}