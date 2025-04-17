export module xe.AssetIO.private_Decompress;

import xe.Core.xeOrdinals;

namespace xe
{
	// compress by zstd
	export bool DecompressZSTD(xeByte* input, xeSize compress_data_size, xeByte* output, xeSize source_size);

	// not compress
	export bool DecompressNone(xeByte* input, xeSize compress_data_size, xeByte* output, xeSize _not_compress_size)
	{
		// If do not compressed this data, this function will copy data from compress data to not compressed data
		// So the comressed size must equal not-compressed size
		std::memcpy(output, input, compress_data_size);
		return true;
	}
}