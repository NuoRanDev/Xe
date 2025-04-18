import xe.AssetsPacking;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

import xe.AssetsPacking.private_Compress;

namespace xe
{
	AssetsPacking::AssetsPacking(xeCompressSolution cmp_solution, xeUint64 AssetFileType)
	{
		file_info_list = { 0 };
		file_info_list.header					= AssetFileType;
		block_data_segment_size					= 0;
		if (cmp_solution == xeCompressSolution::ZSTD)
		{
			file_info_list.compress_solution	= xeCompressSolution::ZSTD;
			Compress_Function					= CompressFunction(CompressZSTD);
		}
		else
		{
			file_info_list.compress_solution	= xeCompressSolution::NONE;
			Compress_Function					= CompressFunction(CompressNone);
		}
	}

	bool AssetsPacking::Write(OpenFileFunction open_cb, WriteFileFunction write_cb, CloseFile close_cb)
	{
		xeSize block_segment_file_info_size = sizeof(XE_ASSET_HEADER) + data_block_info_list.size() * sizeof(xeCompressFileBlockInfo);

		xeSize file_size = sizeof(xeDefaultCompressAssetFileHeaderFormat) + block_segment_file_info_size + block_data_segment_size;

		file_info_list.block_number = data_block_info_list.size();

		XE_INFO_OUTPUT(std::format("Block number : {0}",file_info_list.block_number).c_str());
		XE_INFO_OUTPUT(std::format("Output file size : {0}", file_size).c_str());
		XE_INFO_OUTPUT(std::format("Block segment file info size : {0}", block_segment_file_info_size).c_str());

		if (!open_cb(file_size))
		{
			XE_ERROR_OUTPUT("Open file is failed");
			return false;
		}

		if (!write_cb(reinterpret_cast<const xeByte*>(XE_ASSET_HEADER), sizeof(XE_ASSET_HEADER)))
		{
			XE_ERROR_OUTPUT("Write \"XE_ASSET_HEADER\" is failed");
			return false;
		}

		if (!write_cb(reinterpret_cast<const xeByte*>(&file_info_list), sizeof(file_info_list)))
		{
			XE_ERROR_OUTPUT("Write file info list is failed");
			return false;
		}

		if (!write_cb(reinterpret_cast<const xeByte*>(data_block_info_list.data()), data_block_info_list.size() * sizeof(xeCompressFileBlockInfo)))
		{
			XE_ERROR_OUTPUT("Write data block info list is failed");
			return false;
		}

		for (xeSize i = 0; i < block_data_list.size(); i++)
		{
			// XE_DEBUG_OUTPUT(std::format("Write block : {0}", reinterpret_cast<const char*>(data_block_info_list[i].file_name)).c_str());
			if (!write_cb(block_data_list[i].block_data, block_data_list[i].block_size))
			{
				XE_ERROR_OUTPUT(std::format("Write block : {0} is failed", reinterpret_cast<const char*>(data_block_info_list[i].file_name)).c_str());
				return false;
			}
		}

		close_cb();
		
		return true;
	}

	bool AssetsPacking::AddAsset(xeByte* input_data, xeU8cstr* data_name, xeSize data_size)
	{
		BlockData input_block = { 0 };
		xeCompressFileBlockInfo block_info = { 0 };

		xeSize name_size = std::strlen(reinterpret_cast<const char*>(data_name));

		XE_DEBUG_OUTPUT(std::format("Size: {0}, Name:{1}", data_size, reinterpret_cast<const char*>(data_name)).c_str());

		if (name_size > 512)
		{
			XE_WARNING_OUTPUT("<CLASS: AssetsPacking> File name is too long");
			return false;
		}

		bool state = Compress_Function(input_data, data_size, input_block.block_data, input_block.block_size);
		if (!state)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetsPacking> Add Asset failed");
			return false;
		}
		block_data_list.push_back(input_block);
		block_data_segment_size += input_block.block_size;

		block_info.block_start_behind_block_and_file_header_info = block_data_segment_size;
		block_info.compressed_size = input_block.block_size;
		block_info.source_size = data_size;
		std::memcpy(block_info.file_name, data_name, name_size);

		data_block_info_list.push_back(block_info);

		file_info_list.block_number = data_block_info_list.size();

		XE_DEBUG_OUTPUT(std::format("compressed size: {0} compress {1} %", input_block.block_size, (xeInt32)((float)input_block.block_size / data_size * 100)).c_str());

		return true;
	}

	void AssetsPacking::ReleaseAsset()
	{
		for (auto& block : block_data_list)
		{
			if (block.block_data != nullptr)
			{
				xeFree(block.block_data);
				block.block_data = nullptr;
			}
		}
		block_data_list.clear();
	}

	AssetsPacking::~AssetsPacking()
	{
		ReleaseAsset();
	}
}