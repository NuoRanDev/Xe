#include "XeBasicAsset.h"
#include "decompress/private/private_Decompress.h"
#include "decompress/decompress.h"

namespace xe
{
	bool TestureReader::FirstDecompressFunction(byte_t* compressed_data, int64_t compressed_size, byte_t* not_compressed_data, int64_t not_compressed_size)
	{
		if(!DecompressLZMA(compressed_data, not_compressed_data, compressed_size, not_compressed_size))
		{
			XE_ERROR_OUTPUT("decompress faild!\n");
			return false;
		}
		return true;
	}
	bool TestureReader::SecondDecompressFunction(Testure* out_data, xeColorChannel in_tab)
	{
		return false;
	}
}