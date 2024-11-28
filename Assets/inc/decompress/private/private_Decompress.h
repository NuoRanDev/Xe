#ifndef __PRIVATE__DECOMPRESS_H_
#define __PRIVATE__DECOMPRESS_H_

#include "xeAssetsConfig.h"

namespace xe
{
	// compress by lzma
	bool DecompressLZMA(byte_t* input, byte_t* output, uint64_t compress_data_offset, uint64_t _not_compress_size);
	// compress by zstd
	bool DecompressZSTD(byte_t* input, byte_t* output, uint64_t compress_data_offset, uint64_t _not_compress_size);
	// not compress
	bool DecompressNone(byte_t* input, byte_t* output, uint64_t offset);
}

#endif // __PRIVATE__DECOMPRESS_H_ IS EOF