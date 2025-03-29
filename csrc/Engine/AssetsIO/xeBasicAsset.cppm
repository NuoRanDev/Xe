export module xe.AssetIO.xeBasicAsset;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.AssetIO.xeBasicData;

namespace xe
{
	export class TestureFileReader : public GameDataReader
	{
	public:
		explicit TestureFileReader()
		{
			//CB_decompressfunction = DecompressFunction(&DecompressLZMA);
		}
		std::unique_ptr<TestureFile> GetTesture(const xeInt64 index);
		std::unique_ptr<TestureFile> GetTesture(const char* name);
	};
	export class AudioReader : public GameDataReader
	{
	public:
		explicit AudioReader()
		{
			//CB_decompressfunction = DecompressFunction(&DecompressZSTD);
		}
		std::unique_ptr<AudioFile> GetAudio(const xeInt64 index);
		std::unique_ptr<AudioFile> GetAudio(const char* name);
	};
}