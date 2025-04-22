import xe.AssetIO.xeBasicAsset;

import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

import xe.IO.xeOMmapfstream;

namespace xe
{
	void xe::AssetData::ReleaseAsset()
	{
		xeFree(asset_file_path);
	}

	AssetData::~AssetData()
	{
		ReleaseAsset();
	}

	bool AssetData::ReadAssetFile(const xeU8cstr* file_path, const xeSize string_size)
	{
		bool is_success = true;
		xeByte* file_header = nullptr;

		asset_file_path = xeMalloc<xeU8cstr>(string_size);
		std::memcpy(asset_file_path, file_path, string_size);

		is_success = asset_file_stream.OpenFile(asset_file_path);

		if(!is_success)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetData> Open file failed");
			return false;
		}

		// Read the file header
		file_header = asset_file_stream.GetFstreamPtr<xeByte>(0);
		if (file_header==nullptr)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetData> File is empty");
			return false;
		}

		if (std::memcmp(file_header, XE_ASSET_HEADER, sizeof(XE_ASSET_HEADER)) != 0)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetData> File is broken or may be not xe-asset file");
			return false;
		}

		// Read the block tree header
		is_success = asset_file_stream.FstraemMemcpyOut<xeDefaultCompressAssetFileHeaderFormat>(sizeof(XE_ASSET_HEADER), &file_info_list, sizeof(xeDefaultCompressAssetFileHeaderFormat));
		if (!is_success)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetData> Read the block tree header failed. File is broken or may be not xe-asset file");
			return false;
		}

		// Read the file block info tree
		data_block_info_list.resize(file_info_list.block_number);
		is_success = asset_file_stream.FstraemMemcpyOut<xeCompressFileBlockInfo>
			(sizeof(XE_ASSET_HEADER) + sizeof(xeDefaultCompressAssetFileHeaderFormat),
			data_block_info_list.data(), sizeof(xeCompressFileBlockInfo) * file_info_list.block_number);
		if (!is_success)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetData>  Read the file block info tree Failed. File is broken or may be not xe-asset file");
			return false;
		}

		block_segment_ptr = asset_file_stream.GetFstreamPtr<xeByte>(sizeof(XE_ASSET_HEADER) + sizeof(xeDefaultCompressAssetFileHeaderFormat) + sizeof(xeCompressFileBlockInfo) * file_info_list.block_number);
		return is_success;
	}

	xeInt64 AssetData::GetAssetIndex(const xeU8cstr* file_path)
	{
		for (xeInt64 i = 0; i < static_cast<xeInt64>(data_block_info_list.size()); i++)
		{
			if (data_block_info_list[i] == file_path)
			{
				return i;
			}
		}
		XE_ERROR_OUTPUT(std::format("<CLASS: AssetData> Can't find {0}", reinterpret_cast<const char*>(file_path)).c_str());
		return -1;
	}

	xeByte* AssetData::GetAsset(const xeInt64 index)
	{
		xeByte* compressed_data		= nullptr;
		xeByte* decompressed_data	= nullptr;

		xeInt64 compressed_data_size	= 0;
		xeInt64 decompress_size			= 0;
		xeInt64 _data_size				= 0;

		if (index == -1) return nullptr;

		compressed_data_size	= data_block_info_list[index].compressed_size;
		decompress_size			= data_block_info_list[index].source_size;

		compressed_data			= block_segment_ptr + data_block_info_list[index].block_start_behind_block_and_file_header_info;
		decompressed_data		= xeMalloc<xeByte>(decompress_size);

		if (!AssetCecompressFunction(compressed_data, compressed_data_size, decompressed_data, decompress_size))
		{
			XE_ERROR_OUTPUT(std::format("<CLASS: AssetData> Can't find .Compress solution is {0}", reinterpret_cast<const char*>(GetCompressSolution(file_info_list.compress_solution))).c_str());
			return nullptr;
		}

		return decompressed_data;
	}
}