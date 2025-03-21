export module xe.AssetIO.xeBasicData;

import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeString;
import xe.xeCore.xeAlloc;
import xe.xeCore.xeCoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

import xe.xeIO.xeOMmapfstream;


namespace xe
{
	export using DecompressFunction = std::function<bool(xeByte*, xeUint64, xeByte*, xeUint64)>;
	// 
	export class GameBasicData
	{
	public:

		GameBasicData() = default;

		~GameBasicData()
		{
			BasicRelease();
		}

		void BasicRelease()
		{
			file_info_list = { 0 };
			data_block_info_list.clear();
			data_block_info_list.shrink_to_fit();
		}
	protected:

		// File information
		xeDefaultCompressAssetFileHeaderFormat			file_info_list;

		// File tree
		std::vector<xeCompressFileBlockInfo>			data_block_info_list;

		// 
		xeString										path;
	};

	export class GameDataReader :GameBasicData
	{
	public:
		explicit GameDataReader() :GameBasicData() {}

		bool LoadFile(const char* file_path)
		{
			// Free
			if (data_block_info_list.empty())
			{
				Release();
			}
			//
			fs = new oMmapfstream();
			// Get File Header
			if (!fs->OpenFile(file_path))
			{
				XE_ERROR_OUTPUT(std::format("CAN NOT FIND FILE! FILE {0}", file_path).c_str());
				return false;
			}
			// Read File Header
			if (fs->FstraemStartMemcpyOut<xeDefaultCompressAssetFileHeaderFormat>(&file_info_list, sizeof(xeDefaultCompressAssetFileHeaderFormat)))
			{
				XE_ERROR_OUTPUT(std::format("READING FILE FAILED! FILE {0}", file_path).c_str());
				return false;
			}

			// Read file tree
			xeCompressFileBlockInfo* pdata_block_info_list = fs->GetFstreamPtr<xeCompressFileBlockInfo>(sizeof(xeDefaultCompressAssetFileHeaderFormat));
			if (pdata_block_info_list == nullptr)
			{
				XE_ERROR_OUTPUT(std::format("READING BLOCK DATA INFO FAILED! FILE {0}", file_path).c_str());
				return false;
			}
			data_block_info_list = std::vector<xeCompressFileBlockInfo>(pdata_block_info_list, pdata_block_info_list + file_info_list.file_number);
			return true;
		}

		void Release()
		{
			BasicRelease();
			fs->Release();
		}

		const char* GetDecompressedDataName(xeInt64 index)
		{
			return (const char*)data_block_info_list[index].file_name;
		}

#if defined(EXPORT_C_PLUS_PLUS_API)
		xeInt64 GetDecompressedDataIndex(xeString need_data_block_name)
		{
			return GetDecompressedDataIndex(need_data_block_name.c_str());
		}
#endif // defined EXPORT_C_PLUS_PLUS_API  IS END
		// If want use cpp api 
#if !defined(EXPORT_C_SHARP_API)
	protected:
#endif // defined C_SHARP_API  IS END
		xeInt64 GetDecompressedDataIndex(const char* need_data_block_name)
		{
			// According input name the loop will find data block
			for (xeUint64 i = 0; i < data_block_info_list.size(); i++)
			{
				if (std::strcmp((char*)(data_block_info_list[i].file_name), need_data_block_name) == 0)
				{
					return i;
				}
			}
			// If do not find data block , it will jump the next loop
			// Note warning in CLR
			// And return -1
			XE_WARNING_OUTPUT(std::format("Not find data block:{0}", need_data_block_name).c_str());
			return -1;
		}
	protected:
		// File Stream
		oMmapfstream* fs;
		// Fist, decompress all file
		DecompressFunction CB_decompressfunction;
		//
		xeByte* GetDecompressedData(xeInt64 index, xeUint64& size_out)
		{
			xeByte* compressed_data = nullptr;
			xeByte* not_compressed_data = nullptr;

			xeUint64 compressed_size = data_block_info_list[index]._compress_size;
			xeUint64 not_compressed_size = data_block_info_list[index]._not_compress_size;
			xeUint64 block_start = data_block_info_list[index]._block_start;

			size_out = not_compressed_size;

			compressed_data = fs->GetFstreamPtr<xeByte>(block_start);
			if (compressed_data == nullptr)
			{
				size_out = 0;
				XE_WARNING_OUTPUT(std::format("read data block failed :{0}", index).c_str());
				return nullptr;
			}
			not_compressed_data = new xeByte[not_compressed_size];
			if (!CB_decompressfunction(compressed_data, compressed_size, not_compressed_data, not_compressed_size))
			{
				delete[]not_compressed_data;
				size_out = 0;
				XE_WARNING_OUTPUT("decompress faild!");
				return nullptr;
			}
			return not_compressed_data;
		}
		void FreeDecompressedData(xeByte* decompressed_data)
		{
			decompressed_data = DeleteArray<xeByte>(decompressed_data);
		}
	};

}