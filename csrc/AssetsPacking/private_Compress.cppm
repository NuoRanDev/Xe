export module xe.AssetsPacking.private_Compress;

import xe.Core.xeOrdinals;

namespace xe
{
	// compress by zstd
	export bool CompressZSTD(xeByte* input, xeSize source_size, xeByte*& output, xeSize& compress_data_size);

	// not compress
	export bool CompressNone(xeByte* input, xeSize source_size, xeByte*& output, xeSize&compress_data_size)
	{
		output = input;
		compress_data_size = source_size;
		return true;
	}
}