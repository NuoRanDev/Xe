import xe.AssetIO.private_Decompress;

import xe.Core.xeOrdinals;
import xe.Core.CoreClrOutput;

#include "zstd.h"

namespace xe
{
	// compress by zstd
	bool DecompressZSTD(xeByte* input, xeSize compress_data_size, xeByte* output, xeSize source_size)
	{
		xeSize decompress_frame_size = ZSTD_getFrameContentSize(input, compress_data_size);
		xeSize decompress_size = ZSTD_decompress(output, decompress_frame_size, input, compress_data_size);

		if (decompress_size != source_size)
		{
			XE_WARNING_OUTPUT(std::format("<LIB: ZSTD> Decmpressed file is broken | source size: {0} ,decommpress: {1}", source_size, compress_data_size).c_str());
			return false;
		}
		return true;
	}
}