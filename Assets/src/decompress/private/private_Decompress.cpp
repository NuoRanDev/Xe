#include "decompress/private/private_Decompress.h"
#include "lzma/LzmaDec.hpp"

namespace xe
{
	// compress by lzma
	bool DecompressLZMA(byte_t* input, byte_t* output, uint64_t compress_data_offset, uint64_t _not_compress_size)
	{
		//output = new byte_t[_not_compress_size];
		LzmaALLData* lzma_data = reinterpret_cast<LzmaALLData*>(input);
		uint64_t decompress_size = _not_compress_size;
		if(LzmaUncompress(output, &decompress_size, lzma_data->data, &compress_data_offset, lzma_data->props, PROPS_SIZE)!= SZ_OK)
		{
			XE_ERROR_OUTPUT("Decompressing file Failed\n");
			return false;
		}
		if(decompress_size != _not_compress_size)
		{
			XE_ERROR_OUTPUT(std::format("Decmpressed file is broken | source size: {0} ,decommpress: {1}\n", _not_compress_size, compress_data_offset).c_str());
			return false;
		}
		return true;
	}
	// compress by zstd
	bool DecompressZSTD(byte_t* input, byte_t* output, uint64_t compress_data_offset, uint64_t _not_compress_size)
	{
		return true;
	}
	// not compress
	bool DecompressNone(byte_t* input, byte_t* output, uint64_t offset)
	{
		return true;
	}
}