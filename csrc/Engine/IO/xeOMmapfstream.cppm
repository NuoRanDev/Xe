export module xe.IO.xeOMmapfstream;

import std;

import xe.Core.xeString;
import xe.Core.xeOrdinals;
import xe.Core.CoreClrOutput;


namespace xe
{
	export class BasicMmapfstream
	{
	public:
		BasicMmapfstream() = default;

		template<typename T> T* GetFstreamPtr(size_t offset_byte)
		{
			if (file_size < offset_byte)
			{
				XE_ERROR_OUTPUT(std::format("<Class: BasicMmapfstream> Out of memry : offset size {0}, file size {1}", offset_byte, file_size).c_str());
				return nullptr;
			}
			return (T*)((xeByte*)pfile_start + offset_byte);
		}

		void Release();

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
		bool GetFileSize(const xeU8cstr* file_name);
	};

	export class oMmapfstream final : public BasicMmapfstream
	{
	public:

		template<typename T> bool FstraemStartMemcpyOut(T* dst, size_t number)
		{
			return FstraemMemcpyOut(0, dst, number);
		}

		template<typename T> bool FstraemMemcpyOut(size_t offset_byte, T* dst, size_t number)
		{
			if (file_size < (offset_byte + number * sizeof(T)))
			{
				XE_ERROR_OUTPUT(std::format("<Class: BasicMmapfstream> Out of memry : offset size {0}, file size {1}", sizeof(T) * number, file_size).c_str());
				return false;
			}
			std::memcpy(dst, (xeByte*)pfile_start + offset_byte, number * sizeof(T));
			return true;
		}

#if !defined(EXPORT_C_SHARP_API)
		bool OpenFile(xeString file_name)
		{
			return OpenFile(file_name.data());
		}
#endif // defined(EXPORT_C_SHARP_API)

		bool OpenFile(const xeU8cstr* str);
	};
} // namspace xe is end