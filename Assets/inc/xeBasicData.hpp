#ifndef _BASIC_DATA_HPP_
#define _BASIC_DATA_HPP_

#include "xeAssetsConfig.h"
#include "xeCompressFileStruct.h"

#include "XeCore.hpp"

namespace xe
{
	// 
	class GameBasicData
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
		xeOtherCompressFileHeaderFormat					file_info_list;

		// File tree
		std::vector<xeCompressFileBlockInfo>			data_block_info_list;

		// 
		std::string										path;
	};
	template<typename out_type, class type_tab>
	class GameDataReader :GameBasicData
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
			if (!fs->GetFilePtr(file_path))
			{
				//XE_ERROR_OUTPUT(std::format("CAN NOT FIND FILE! FILE {0}", file_path).c_str());
				return false;
			}
			// Read File Header
			if(fs->FstraemStartMemcpyOut(&file_info_list, sizeof(xeOtherCompressFileHeaderFormat)))
			{
				//XE_ERROR_OUTPUT(std::format("READING FILE FAILED! FILE {0}", file_path).c_str());
				return false;
			}
			// Read file tree
			xeCompressFileBlockInfo* pdata_block_info_list = fs->GetFstreamPtr<xeCompressFileBlockInfo>(sizeof(xeOtherCompressFileHeaderFormat));
			if (pdata_block_info_list == nullptr)
			{
				//XE_ERROR_OUTPUT(std::format("READING BLOCK DATA INFO FAILED! FILE {0}", file_path).c_str());
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

#if defined(EXPORT_C_SHARP_API)
		out_type* GetDataPtr(CsharpString* need_data_block, uint64_t number)
		{
			out_type* poutput_testures = new Testure[number];
			byte_t* compressed_data = nullptr;
			byte_t* not_compressed_data = nullptr;

			uint64_t compressed_size = 0;
			uint64_t not_compressed_size = 0;
			uint64_t block_start = 0;
			for (uint64_t i = 0; i < number; i++)
			{
				// According input name the loop will find data block
				for (auto& data_block_info : data_block_info_list)
				{
					if (strcmp((char*)(data_block_info.file_name), (char*)(need_data_block[i].data)) == 0)
					{
						compressed_size = data_block_info._compress_size;
						not_compressed_size = data_block_info._not_compress_size;
						block_start = data_block_info._block_start;
						goto IS_FIND_FILE;
					}
				}
				// If do not find data block , it will jump the next loop
				// Note warning in CLR
				XE_WARNING_OUTPUT(std::format("Not find data block:{0}", (char*)(need_data_block[i].data)).c_str());
				poutput_testures[i] = { 0 };
				continue;
				// If found ,get data type
			IS_FIND_FILE:
				compressed_data = fs->GetFstreamPtr<byte_t>(block_start);
				if (compressed_data == nullptr)
				{
					XE_WARNING_OUTPUT(std::format("read data block failed :{0}", (char*)(need_data_block[i].data)).c_str());
					poutput_testures[i] = { 0 };
					continue;
				}
				not_compressed_data = new byte_t[not_compressed_size];
				if(!FirstDecompressFunction(compressed_data, compressed_size, not_compressed_data, not_compressed_size))
				{
					delete[]not_compressed_data;
					XE_WARNING_OUTPUT("first compress faild!");
					continue;
				}
				if (!SecondDecompressFunction())
				{
					delete[]not_compressed_data;
					XE_WARNING_OUTPUT("second compressed faild!");
					continue;
				}
			}
		}
#endif // defined C_SHARP_API  IS END

#if defined(EXPORT_C_PLUS_PLUS_API)
		std::vector<out_type> GetDataVector(std::vector<std::string>& file_name_list)
		{}
#endif // defined EXPORT_C_PLUS_PLUS_API  IS END
	protected:
		// File Stream
		oMmapfstream*										fs;
		// Fist, decompress all file
		virtual bool FirstDecompressFunction(byte_t* compressed_data, int64_t compressed_size, byte_t* not_compressed_data, int64_t not_compressed_size)
		{
			// If do not compressed this data, this function will copy data from compress data to not compressed data
			// So the comressed size must equal not-compressed size
			memcpy(not_compressed_data, compressed_data, compressed_size);
			return true;
		}
		// Second, according to file-type or file name decompress a single file
		virtual bool SecondDecompressFunction(out_type* out_data, type_tab in_tab)
		{
			return false;
		}
	};
}
#endif // _BASIC_DATA_H_ IS EOF