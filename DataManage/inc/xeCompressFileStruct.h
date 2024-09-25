#ifndef _INC_XE_COMPRESS_FILE_STRUCT_H_
#define _INC_XE_COMPRESS_FILE_STRUCT_H_

#include "xeDataManageConfig.h"

namespace gamedb
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

		constexpr const uint64_t xeGetCompressType(const char data[8])
		{
			uint64_t output = 0;
			for (int i = 0; i < 8; i++)
			{
				output = output << 8 + data[i];
			}
			return output;
		}

		// ASSET FILE HEADER
		uint64_t IMAGE_ASSET_FILE_HEADER	= xeGetCompressType("IMAGE\0\0");
		uint16_t AUDIO_ASSET_FILE_HEADER	= xeGetCompressType("AUDIO\0\0");
		uint64_t VIDEO_ASSET_FILE_HEADER	= xeGetCompressType("VIDEO\0\0");
		uint64_t TEXT_ASSET_FILE_HEADER		= xeGetCompressType("TEXT\0\0\0");
		uint64_t MODEL_ASSET_FILE_HEADER	= xeGetCompressType("MODEL\0\0");
		uint64_t DATABASE_ASSET_FILE_HEADER = xeGetCompressType("DB\0\0\0\0\0");
		uint64_t OTHER_ASSET_FILE_HEADER	= 0;
		};

	// File block eare
	// start offset and size
	struct FileBlockInfo
	{
		char8_t file_name[512];
		offsetptr_t _start;
		int64_t _size;
	};

	// file pack truction
	struct xeFilePackFormat
	{
		uint64_t file_number;
		FileBlockInfo* file_tree;
		byte_t file_data[];
	};

	// Image File support jpge png
	struct xeImageCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::IMAGE_ASSET_FILE_HEADER;
		// Image compressing's solution is defaulted the LZMA
		xeCompressSolution compress_solution = xeCompressSolution::LZMA;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// Audio File support ogg flac mp3
	struct xeAudioCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::AUDIO_ASSET_FILE_HEADER;
		// Audio compressing's solution is defaulted the NONE
		xeCompressSolution compress_solution = xeCompressSolution::NONE;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// Video file suppor mp4 flv webp
	struct xeVideoCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::VIDEO_ASSET_FILE_HEADER;
		// Video compressing's solution is defaulted the NONE
		xeCompressSolution compress_solution = xeCompressSolution::NONE;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// Shader File suppert spir-v glsl hlsl
	struct xeShaderCompressFileFormat
	{
		// File Info
		const char header[8] = "SHADER\0";
		// Shader compressing's solution is defaulted the ZSTD
		xeCompressSolution compress_solution = xeCompressSolution::ZSTD;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// TEXT File must encode utf-8
	struct xeTextCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::TEXT_ASSET_FILE_HEADER;
		// Text compressing's solution is defaulted the ZSTD
		xeCompressSolution compress_solution = xeCompressSolution::ZSTD;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// Model File suppert obj stl xml json
	struct xeModelCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::MODEL_ASSET_FILE_HEADER;
		// Model compressing's solution is defaulted the ZSTD
		xeCompressSolution compress_solution = xeCompressSolution::ZSTD;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// DataBase File suppert db bin
	struct xeDataBaseCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::DATABASE_ASSET_FILE_HEADER;
		// DataBase compressing's solution is defaulted the ZSTD
		xeCompressSolution compress_solution = xeCompressSolution::ZSTD;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};

	// DataBase File suppert db bin
	struct xeOtherCompressFileFormat
	{
		// File Info
		uint64_t header = xeAssetFileType::OTHER_ASSET_FILE_HEADER;
		// Other compressing's solution is defaulted the ZSTD
		xeCompressSolution compress_solution;
		uint64_t file_number;
		// byte data
		byte_t file_data[];
	};
}

#endif // _INC_XE_COMPRESS_FILE_STRUCT_H_ IS EOF