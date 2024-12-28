#ifndef _XE_BASIC_ASSET_H_
#define _XE_BASIC_ASSET_H_

#include "xeAssetsConfig.h"
#include "xeBasicData.hpp"

#include "decompress/decompress.h"

namespace xe
{
	class XE_ASSETS_EXPORT_C_PLUS_PLUS_API TestureFileReader :GameDataReader
	{
	public:
		explicit TestureFileReader()
		{
			CB_decompressfunction = DecompressFunction(&DecompressLZMA);
		}
		TestureFile* GetTesture(const int64_t index);
		TestureFile* GetTesture(const char* name);
	};
	class XE_ASSETS_EXPORT_C_PLUS_PLUS_API AudioReader :GameDataReader
	{
	public:
		explicit AudioReader()
		{
			CB_decompressfunction = DecompressFunction(&DecompressZSTD);
		}
		AudioFile* GetAudio(const int64_t index);
		AudioFile* GetAudio(const char* name);
	};
}

#endif // _XE_BASIC_ASSET_H_ IS EOF