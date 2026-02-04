#include "filesystem/xeFileMmapStream.hpp"

#include "log/xeLogOutput.hpp"

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif // OS inc

#include <filesystem>

namespace xe
{
	const byte_t *Mmapfstream::get_mmap_offset_ptr(size_t offset_byte) const noexcept
	{
		if (file_size < offset_byte)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
							  "xeCore",
							  std::format("Out of memory : offset size {0}, file size {1}", offset_byte, file_size).c_str());
			return nullptr;
		}
		return (byte_t *)pfile_start + offset_byte;
	}

	bool Mmapfstream::get_file_size(const utf8_t *path) noexcept
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

	bool Mmapfstream::read(size_t offset_byte, size_t data_size, any_type_ptr_t dst) const noexcept
	{
		if (file_size < offset_byte + data_size)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
							  "xeCore",
							  std::format("Out of memory : offset size {0}, file size {1}", offset_byte + data_size, file_size).c_str());
			return false;
		}
		std::memcpy(dst, (byte_t *)pfile_start + offset_byte, data_size);
		return true;
	}

	bool Mmapfstream::open_file(const Path &path) noexcept
	{
		if (!get_file_size(path.path_str.data()))
			return false;
#if defined(_WIN32)
		c_dump_file_descriptor = CreateFileW(
			path.get_native_str(),
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
#elif defined(__linux__)
		fd = open(reinterpret_cast<const char*>(path.get_native_str()),O_RDONLY);
		if(fd == -1){
			XE_WARNING_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				std::format("Create file mapping failed , SYSTEM ERROR CODE:{0}", errno).c_str());
			return false;
		}
		// 获取文件参数
		fstat(fd, &st);
		// PROT_READ : 映射区可读
		// MAP_PRIVATE : 修改内存文件并不会修改底层数据，修改文件仅当前进程有效
		pfile_start = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE,fd,0);
		if (pfile_start == MAP_FAILED){
			XE_WARNING_OUTPUT(
				XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				std::format("Map file mapping failed ,SYSTEM ERROR CODE:{0}", errno).c_str());
				close(fd);
			return false;
		}
#endif // _WIN32 FUNCTION IS END
		return true;
	}

	void Mmapfstream::release() noexcept
	{
#if defined(_WIN32)
		if (pfile_start != nullptr)
			UnmapViewOfFile(pfile_start);
		if (hfile_mapping != nullptr)
			CloseHandle(hfile_mapping);
		if (c_dump_file_descriptor != nullptr)
			CloseHandle(c_dump_file_descriptor);
		pfile_start = nullptr;
		hfile_mapping = nullptr;
		file_size = 0;
#elif defined(__linux__)
		if (pfile_start != MAP_FAILED){
			munmap(pfile_start, st.st_size);
			close(fd);
		}
#endif // _WIN32 FUNCTION IS END
	}
} // namespace xe is end