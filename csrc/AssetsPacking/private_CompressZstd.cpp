import xe.AssetsPacking.private_Compress;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;

#include "zstd.h"

namespace xe
{
	bool CompressZSTD(xeByte* input, xeSize source_size, xeByte*& output, xeSize& compress_data_size)
	{
		xeSize buffer_size = ZSTD_compressBound(source_size);
		output = xeMalloc<xeByte>(buffer_size);
		compress_data_size = ZSTD_compress(output, buffer_size, input, source_size, ZSTD_lazy);

		auto code = ZSTD_isError(compress_data_size);
		if (code) 
		{
			XE_ERROR_OUTPUT(std::format("<LIB: ZSTD> CompressZSTD error: {0}", ZSTD_getErrorName(compress_data_size)).c_str());
			return false;
		}
		xeRealloc<xeByte>(output, compress_data_size);
		return true;
	}
}