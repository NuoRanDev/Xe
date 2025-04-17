export module xe.AssetIO.xeAssetType;

import xe.Core.xeBaseDataType;
import xe.Core.xeOrdinals;

import xe.AssetIO.xeBasicAsset;

namespace xe
{
	export class TestureEncodedDataReader : public AssetData
	{
	public:
		explicit TestureEncodedDataReader() = default;

		std::unique_ptr<TestureEncodedData> GetTesture(const xeInt64 index);
#if !defined(EXPORT_C_SHARP_API)
		std::unique_ptr<TestureEncodedData> GetTesture(const xeString file_path)
		{
			return GetTesture(file_path.data());
		}
	private:
#endif // defined(EXPORT_C_SHARP_API)
		std::unique_ptr<TestureEncodedData> GetTesture(const xeU8cstr* file_path)
		{
			return GetTesture(GetAssetIndex(file_path));
		}
	};
	export class AudioEncodedDataReader : public AssetData
	{
	public:
		explicit AudioEncodedDataReader() = default;

		std::unique_ptr<AudioEncodedData> GetAudio(const xeInt64 index);

#if !defined(EXPORT_C_SHARP_API)
		std::unique_ptr<AudioEncodedData> GetAudio(const xeString file_path)
		{
			return GetAudio(file_path.data());
		}
	private:
#endif // defined(EXPORT_C_SHARP_API)
		std::unique_ptr<AudioEncodedData> GetAudio(const xeU8cstr* name)
		{
			return GetAudio(GetAssetIndex(name));
		}
	};
}