export module xe.AssetsPacking;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

namespace xe
{
	export using CompressFunction	= std::function<bool(xeByte*, xeSize, xeByte*&, xeSize&)>;

	export struct BlockData
	{
		xeUint64		block_size;
		xeByte*			block_data;
	};

	export using OpenFileFunction	= std::function<bool(const xeU8cstr*)>;
	export using WriteFileFunction	= std::function<bool(const xeByte*, const xeSize)>;
	export using CloseFile			= std::function<bool(void)>;

	export class AssetsPacking
	{
	public:
		explicit AssetsPacking(xeCompressSolution cmp_solution, xeUint64 AssetFileType);

		bool Write(const xeU8cstr* file_path, OpenFileFunction open_cb, WriteFileFunction write_cb, CloseFile close_cb);

		bool AddAsset(xeByte* input_data, xeSize data_size);

		void ReleaseAsset();

		~AssetsPacking();

	private:
		// File information
		xeDefaultCompressAssetFileHeaderFormat			file_info_list;

		// File tree
		std::vector<xeCompressFileBlockInfo>			data_block_info_list;

		// Block list
		std::vector<BlockData>							block_list;

		//
		CompressFunction								Compress_Function;
	};
} // namespace xe