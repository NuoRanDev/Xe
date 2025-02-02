#ifndef _INC_XE_COMPRESS_FILE_STRUCT_HPP_
#define _INC_XE_COMPRESS_FILE_STRUCT_HPP_

#include "xeAssetsConfig.hpp"

namespace xe
{
	// Compreessing solution
	// ZSTD: ztsdland LZMA: lzma2
	enum class xeCompressSolution :uint32_t
	{
		NONE = 0,
		ZSTD = 1,
		LZMA = 2
	};

	namespace xeAssetFileType
	{

		constexpr uint64_t xeGetCompressType(const char data[8])
		{
			uint64_t output = 0;
			for (int i = 0; i < 8; i++)
			{
				output = (output << 8) + data[i];
			}
			return output;
		}

		// ASSET FILE HEADER
		const uint64_t IMAGE_ASSET_FILE_HEADER		= xeGetCompressType("IMAGE\0\0");
		const uint64_t AUDIO_ASSET_FILE_HEADER		= xeGetCompressType("AUDIO\0\0");
		const uint64_t VIDEO_ASSET_FILE_HEADER		= xeGetCompressType("VIDEO\0\0");
		const uint64_t TEXT_ASSET_FILE_HEADER		= xeGetCompressType("TEXT\0\0\0");
		const uint64_t MODEL_ASSET_FILE_HEADER		= xeGetCompressType("MODEL\0\0");
		const uint64_t SHADER_ASSET_FILE_HEADER		= xeGetCompressType("SHADER\0");
		const uint64_t OTHER_ASSET_FILE_HEADER		= 0;
	};

	// File block eare
	// start offset and size
	struct xeCompressFileBlockInfo
	{
		uint8_t file_name[512];
		uint64_t _block_start;
		uint64_t _compress_size;
		uint64_t _not_compress_size;
	};


	template <uint64_t load_header, xeCompressSolution load_compress_solution>
	struct xeCompressAssetFileHeaderFormat
	{
		// File Info
		uint64_t header = load_header;

		// Compressing's solution
		xeCompressSolution compress_solution = load_compress_solution;

		uint64_t file_number;
	};

	// Default File
	using xeDefaultCompressAssetFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::OTHER_ASSET_FILE_HEADER, xeCompressSolution::NONE>;

	// Image File support jpge png
	// Image compressing's solution is defaulted the LZMA
	using xeImageCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::IMAGE_ASSET_FILE_HEADER, xeCompressSolution::LZMA>;

	// Audio File support ogg flac
	// Audio compressing's solution is defaulted the NONE
	using xeAudioCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::AUDIO_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// Video file suppor mp4 webp
	// Video compressing's solution is defaulted the NONE
	using xeVideoCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::VIDEO_ASSET_FILE_HEADER, xeCompressSolution::NONE>;

	// Shader File suppert spir-v
	// Shader compressing's solution is defaulted the ZSTD
	using xeShaderCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::SHADER_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// TEXT File must encode utf-8
	// DataBase compressing's solution is defaulted the ZSTD
	using xeTextCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::TEXT_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// Model File suppert obj stl xml json
	// DataBase compressing's solution is defaulted the ZSTD
	using xeModelCompressFileHeaderFormat = xeCompressAssetFileHeaderFormat<xeAssetFileType::MODEL_ASSET_FILE_HEADER, xeCompressSolution::ZSTD>;

	// DataBase File suppert csv bin
	//using xeDataBaseCompressFileHeaderFormat = xeTextCompressFileHeaderFormat;
}

#endif // _INC_XE_COMPRESS_FILE_STRUCT_HPP_ IS EOF