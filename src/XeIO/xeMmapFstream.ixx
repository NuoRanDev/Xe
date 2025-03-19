export module xeIO.xeMmapFstream;

import std;
import xeCore.xeOrdinals;
import xeCore.xeCoreClrOutput;

#if defined(_WIN32)
#include <Windows.h>
#endif // OS inc


namespace xe
{
	class BasicMmapfstream
	{
	public:
		BasicMmapfstream() = default;

		virtual bool GetFilePtr(const char* str) {}

		template<typename T> T* GetFstreamPtr(size_t offset_byte)
		{
			if (file_size < offset_byte)
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (T*)((byte_t*)pfile_start + offset_byte);
		}

		void Release()
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

		~BasicMmapfstream()
		{
			Release();
		}

		xeUint64 file_size;

	protected:
		void* pfile_start;
#if defined(_WIN32)
		void* hfile_mapping;
		void* c_dumpFileDescriptor;
#elif defied(__linux__)
#else
#error "SUPPORTED ERROR: NOT SUPPORT THIS SYSTEM!"
#endif // _WIN32
		bool GetFileSize(const char* file_name)
		{
			if (!std::filesystem::exists(file_name))
			{
				XE_WARNING_OUTPUT("CAN'T FIND FILE");
				return false;
			}
			file_size = std::filesystem::file_size(file_name);
			return true;
		}
	};

	export class oMmapfstream : public BasicMmapfstream
	{
	public:
		virtual bool GetFilePtr(const char* str) final
		{
			if(!GetFileSize(str))
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

		template<typename T> bool FstraemStartMemcpyOut(T* dst, size_t number)
		{
			if (file_size < number * sizeof(T))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			std::memcpy(dst, pfile_start, sizeof(T) * number);
			return true;
		}

		template<typename T> bool FstraemMemcpyOut(T* dst, size_t offset_byte, size_t number)
		{
			if (file_size < (offset_byte + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			std::memcpy(dst, (xeByte*)pfile_start + offset_byte, number * sizeof(T));
			return true;
		}
	};
} // namspace xe is end