#ifndef _XE_BASIC_ASSET_H_
#define _XE_BASIC_ASSET_H_

#include "xeAssetsConfig.h"
#include "XeBasicData.hpp"

namespace xe
{
	class TestureReader :GameDataReader
	{
	public:
		Testure* GetTesture(const int64_t index);
	protected:
		virtual bool DecompressFunction(byte_t* compressed_data, int64_t compressed_size, byte_t* not_compressed_data, int64_t not_compressed_size);
	};
}

#endif // _XE_BASIC_ASSET_H_ IS EOF