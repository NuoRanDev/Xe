export module xe.AssetIO.private_Decompress;

import xe.Core.xeOrdinals;

namespace xe
{
	// compress by lzma
	export bool DecompressLZMA(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size);
	// compress by zstd
	export bool DecompressZSTD(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size);
	// not compress
	export bool DecompressNone(xeByte* input, xeUint64 compress_data_size, xeByte* output, xeUint64 _not_compress_size);
}