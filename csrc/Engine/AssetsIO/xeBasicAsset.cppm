export module xe.AssetIO.xeBasicAsset;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.AssetIO.xeBasicData;

namespace xe
{
	export class TestureEncodedDataReader : public GameDataReader
	{
	public:
		explicit TestureEncodedDataReader()
		{
			//CB_decompressfunction = DecompressFunction(&DecompressLZMA);
		}
		std::unique_ptr<TestureEncodedData> GetTesture(const xeInt64 index);
		std::unique_ptr<TestureEncodedData> GetTesture(const char* name);
	};
	export class AudioEncodedDataReader : public GameDataReader
	{
	public:
		explicit AudioEncodedDataReader()
		{
			//CB_decompressfunction = DecompressFunction(&DecompressZSTD);
		}
		std::unique_ptr<AudioEncodedData> GetAudio(const xeInt64 index);
		std::unique_ptr<AudioEncodedData> GetAudio(const char* name);
	};
}