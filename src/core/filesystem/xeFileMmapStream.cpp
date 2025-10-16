#include "filesystem/xeFileMmapStream.hpp"

#if defined(_WIN32)
#include <Windows.h>
#endif // OS inc

#include <filesystem>

namespace xe
{
	bool Mmapfstream::get_file_size(const utf8_t* path) noexcept
	{
		if (!std::filesystem::exists(path))
		{
			XE_INFO_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				"Can't find file");
			return false;
		}
		file_size = std::filesystem::file_size(path);
		return true;
	}

	bool Mmapfstream::open_file(const utf8_t* path) noexcept
	{
		if (!get_file_size(path))
			return false;
#ifdef _WIN32
		c_dump_file_descriptor = CreateFileA(
			reinterpret_cast<const char*>(path),
			GENERIC_READ,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (c_dump_file_descriptor == nullptr)
		{
			XE_WARNING_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				std::format("Create file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return false;
		}
		hfile_mapping = CreateFileMapping(c_dump_file_descriptor, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hfile_mapping == nullptr)
		{
			XE_WARNING_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				std::format("Open file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return false;
		}
		pfile_start = MapViewOfFile(hfile_mapping, FILE_MAP_READ, 0, 0, 0);
		if (pfile_start == nullptr)
		{
			XE_WARNING_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				std::format("Map file mapping failed ,SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			CloseHandle(hfile_mapping);
			return false;
		}
#endif // _WIN32 FUNCTION IS END
		return true;
	}

	void Mmapfstream::release() noexcept
	{
#ifdef _WIN32
		if (pfile_start != nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping != nullptr)
			CloseHandle(hfile_mapping);
		if (c_dump_file_descriptor != nullptr)
			CloseHandle(c_dump_file_descriptor);
#endif // _WIN32 FUNCTION IS END
		pfile_start = nullptr;
		hfile_mapping = nullptr;
		file_size = 0;
	}
} // namespace xe is end