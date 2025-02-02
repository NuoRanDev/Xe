#include "decompress/private_Decompress.hpp"
#include "lzma/LzmaDec.hpp"
#include "zstd.h"

namespace xe
{
	// compress by lzma
	bool DecompressLZMA(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size)
	{
		//output = new byte_t[_not_compress_size];
		LzmaALLData* lzma_data = reinterpret_cast<LzmaALLData*>(input);
		uint64_t decompress_size = _not_compress_size;
		if (LzmaUncompress(output, &decompress_size, lzma_data->data, &compress_data_size, lzma_data->props, PROPS_SIZE) != SZ_OK)
		{
			XE_WARNING_OUTPUT("[LZMA] Decompressing file Failed");
			return false;
		}
		if(decompress_size != _not_compress_size)
		{
			XE_WARNING_OUTPUT(std::format("[LZMA] Decmpressed file is broken | source size: {0} ,decommpress: {1}", _not_compress_size, compress_data_size).c_str());
			return false;
		}
		return true;
	}
	// compress by zstd
	bool DecompressZSTD(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size)
	{
		uint64_t decompress_size = ZSTD_getFrameContentSize(input, compress_data_size);
		if (decompress_size != _not_compress_size)
		{
			XE_WARNING_OUTPUT(std::format("[ZSTD] Decmpressed file is broken | source size: {0} ,decommpress: {1}", _not_compress_size, compress_data_size).c_str());
			return false;
		}
		if (ZSTD_decompress(output, decompress_size, input, compress_data_size) != _not_compress_size)
		{
			XE_WARNING_OUTPUT("[ZSTD] Decompressing file Failed");
			return false;
		}
		return true;
	}
	// not compress
	bool DecompressNone(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size)
	{
		// If do not compressed this data, this function will copy data from compress data to not compressed data
		// So the comressed size must equal not-compressed size
		memcpy(output, input, compress_data_size);
		return true;
	}
}