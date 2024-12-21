#ifndef _INC_PRIVATE_DECOMPRESS_H_
#define _INC_PRIVATE_DECOMPRESS_H_

#include "xeAssetsConfig.h"

namespace xe 
{
	// compress by lzma
	bool DecompressLZMA(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size);
	// compress by zstd
	bool DecompressZSTD(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size);
	// not compress
	bool DecompressNone(byte_t* input, uint64_t compress_data_size, byte_t* output, uint64_t _not_compress_size);
}

#endif // _INC_PRIVATE_DECOMPRESS_H_ IS EOF