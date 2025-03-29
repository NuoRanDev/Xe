export module xe.AssetIO.xeCompressFileStruct;

import xe.Core.xeOrdinals;

namespace xe
{
	// Compreessing solution
	// ZSTD: ztsdland LZMA: lzma2
	export enum class xeCompressSolution :xeUint32
	{
		NONE = 0,
		ZSTD = 1,
		LZMA = 2
	};

	export namespace xeAssetFileType
	{

		constexpr xeUint64 xeGetCompressType(const char data[8])
		{
			xeUint64 output = 0;
			for (int i = 0; i < 8; i++)
			{
				output = (output << 8) + data[i];
			}
			return output;
		}

		// ASSET FILE HEADER
		const xeUint64 IMAGE_ASSET_FILE_HEADER	= xeGetCompressType("IMAGE\0\0");
		const xeUint64 AUDIO_ASSET_FILE_HEADER	= xeGetCompressType("AUDIO\0\0");
		const xeUint64 VIDEO_ASSET_FILE_HEADER	= xeGetCompressType("VIDEO\0\0");
		const xeUint64 TEXT_ASSET_FILE_HEADER	= xeGetCompressType("TEXT\0\0\0");
		const xeUint64 MODEL_ASSET_FILE_HEADER	= xeGetCompressType("MODEL\0\0");
		const xeUint64 SHADER_ASSET_FILE_HEADER = xeGetCompressType("SHADER\0");
		const xeUint64 OTHER_ASSET_FILE_HEADER	= 0;
	};

	// File block eare
	// start offset and size
	export struct xeCompressFileBlockInfo
	{
		xeU8cstr file_name[512];
		xeUint64 _block_start;
		xeUint64 _compress_size;
		xeUint64 _not_compress_size;
	};


	export template <xeUint64 load_header, xeCompressSolution load_compress_solution> struct xeCompressAssetFileHeaderFormat
	{
		// File Info
		xeUint64 header = load_header;

		// Compressing's solution
		xeCompressSolution compress_solution = load_compress_solution;

		xeUint64 file_number;
	};

	// Default File
	export using xeDefaultCompressAssetFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::OTHER_ASSET_FILE_HEADER, xeCompressSolution::NONE>;

	// Image File support jpge png
	// Image compressing's solution is defaulted the LZMA
	export using xeImageCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::IMAGE_ASSET_FILE_HEADER, xeCompressSolution::LZMA>;

	// Audio File support ogg flac
	// Audio compressing's solution is defaulted the NONE
	export using xeAudioCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::AUDIO_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// Video file suppor mp4 webp
	// Video compressing's solution is defaulted the NONE
	export using xeVideoCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::VIDEO_ASSET_FILE_HEADER, xeCompressSolution::NONE>;

	// Shader File suppert spir-v
	// Shader compressing's solution is defaulted the ZSTD
	export using xeShaderCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::SHADER_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// TEXT File must encode utf-8
	// DataBase compressing's solution is defaulted the ZSTD
	export using xeTextCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::TEXT_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// Model File suppert obj stl xml json
	// DataBase compressing's solution is defaulted the ZSTD
	export using xeModelCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::MODEL_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// DataBase File suppert csv bin
	//using xeDataBaseCompressFileHeaderFormat = xeTextCompressFileHeaderFormat;
}