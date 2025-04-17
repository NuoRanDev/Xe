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
		file_info_list.block_number				= AssetFileType;
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

	bool AssetsPacking::Write(const xeU8cstr* file_path, OpenFileFunction open_cb, WriteFileFunction write_cb, CloseFile close_cb)
	{
		if(!open_cb(file_path))
		{
			XE_ERROR_OUTPUT(std::format("Open file : {0} is failed", reinterpret_cast<const char*>(file_path)).c_str());
		}
		
		return true;
	}

	bool AssetsPacking::AddAsset(xeByte* input_data, xeSize data_size)
	{
		BlockData input_block = {0};
		bool state = Compress_Function(input_data, data_size, input_block.block_data, input_block.block_size);
		if (state)
		{
			XE_ERROR_OUTPUT("<CLASS: AssetsPacking> Add Asset failed");
			return false;
		}
		block_list.push_back(input_block);
		return true;
	}

	void AssetsPacking::ReleaseAsset()
	{
		for (auto& block : block_list)
		{
			if (block.block_data != nullptr)
			{
				xeFree(block.block_data);
				block.block_data = nullptr;
			}
		}
		block_list.clear();
	}

	AssetsPacking::~AssetsPacking()
	{
		ReleaseAsset();
	}
}