#ifndef _XE_BASIC_ASSET_H_
#define _XE_BASIC_ASSET_H_

#include "xeAssetsConfig.h"
#include "XeBasicData.hpp"

namespace xe
{
	class TestureReader :GameDataReader<Testure, xeColorChannel>
	{
	protected:
		virtual bool FirstDecompressFunction(byte_t* compressed_data, int64_t compressed_size, byte_t* not_compressed_data, int64_t not_compressed_size);
		virtual bool SecondDecompressFunction(Testure* out_data, xeColorChannel in_tab);
	};
}

#endif // _XE_BASIC_ASSET_H_ IS EOF