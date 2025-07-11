export module xe.AssetIO.xeBasicAsset;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

import xe.AssetIO.xeCompressFileStruct;

import xe.IO.xeOMmapfstream;

#if !defined(EXPORT_C_SHARP_API)
#endif // defined(EXPORT_C_SHARP_API)

namespace xe
{
	/*
	* @param first xeBye* : compressed data
	* @param first xeSize : compressed data size
	* @param second xeBye* : decompressed data
	* @param second xeSize : decompressed data size
	*/
	export using DecompressFunction = std::function<bool(xeByte*, xeSize, xeByte*, xeSize)>;

	// 
	export class AssetData
	{
	public:

		AssetData() = default;

		void ReleaseAsset();

		~AssetData();

		xeU8cstr* GetAssetName(xeUint64 index) 
		{
			return data_block_info_list[index].file_name;
		}

#if !defined(EXPORT_C_SHARP_API)
		// Load the asset file
		bool ReadAssetFile(xeString file_path) 
		{
			return ReadAssetFile(file_path.data(),file_path.CharacterDataSize());
		}

		xeInt64 GetAssetIndex(xeString file_path)
		{
			return GetAssetIndex(file_path.data());
		}

	protected:
		xeByte* GetAsset(xeString file_path)
		{
			return GetAsset(file_path.data());
		}
#endif // defined(EXPORT_C_SHARP_API)

		// Load the asset file by c style string
		bool ReadAssetFile(const xeU8cstr* file_path, const xeSize string_size);

		xeInt64 GetAssetIndex(const xeU8cstr* file_path);

		xeByte* GetAsset(const xeU8cstr* file_path)
		{
			return GetAsset(GetAssetIndex(file_path));
		}

		xeByte* GetAsset(const xeInt64 index);
		
	protected:

		// File information
		xeDefaultCompressAssetFileHeaderFormat			file_info_list;

		// File tree
		std::vector<xeCompressFileBlockInfo>			data_block_info_list;

		// Assets files path
		xeU8cstr*										asset_file_path;

		// file stream
		xe::oMmapfstream 								asset_file_stream;

		// decompress function
		DecompressFunction								AssetCecompressFunction;

		// Block Segment ptr
		xeByte* 										block_segment_ptr;
	};
}