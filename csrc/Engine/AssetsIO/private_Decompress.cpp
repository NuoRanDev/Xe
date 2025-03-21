import xe.AssetIO.private_Decompress;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeCoreClrOutput;

#include "zstd.h"

namespace xe
{
	// compress by lzma
	bool DecompressLZMA(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size)
	{
		return true;
	}
	// compress by zstd
	bool DecompressZSTD(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size)
	{
		xeUint64 decompress_size = ZSTD_getFrameContentSize(input, compress_data_size);
		if (decompress_size != _not_compress_size)
		{
			XE_WARNING_OUTPUT(std::format("LIB <ZSTD> Decmpressed file is broken | source size: {0} ,decommpress: {1}", _not_compress_size, compress_data_size).c_str());
			return false;
		}
		if (ZSTD_decompress(output, decompress_size, input, compress_data_size) != _not_compress_size)
		{
			XE_WARNING_OUTPUT("LIB <ZSTD> Decompressing file Failed");
			return false;
		}
		return true;
	}
	// not compress
	bool DecompressNone(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size)
	{
		// If do not compressed this data, this function will copy data from compress data to not compressed data
		// So the comressed size must equal not-compressed size
		std::memcpy(output, input, compress_data_size);
		return true;
	}
}