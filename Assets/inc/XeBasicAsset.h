#ifndef _XE_BASIC_ASSET_H_
#define _XE_BASIC_ASSET_H_

#include "xeAssetsConfig.h"
#include "XeBasicData.hpp"

#include "decompress/decompress.h"

namespace xe
{
	class XE_ASSETS_EXPORT_C_PLUS_PLUS_API TestureReader :GameDataReader
	{
	public:
		explicit TestureReader()
		{
			CB_decompressfunction = DecompressFunction(&DecompressLZMA);
		}
		Testure* GetTesture(const int64_t index);
		Testure* GetTesture(const char* name);
	};
	class XE_ASSETS_EXPORT_C_PLUS_PLUS_API AudioReader :GameDataReader
	{
	public:
		explicit AudioReader()
		{
			CB_decompressfunction = DecompressFunction(&DecompressZSTD);
		}
		Audio* GetAudio(const int64_t index);
		Audio* GetAudio(const char* name);
	};
}

#endif // _XE_BASIC_ASSET_H_ IS EOF