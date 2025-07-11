import xe.AssetIO.xeAssetType;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeAlloc;

//import xe.AssetIO.xeBasicData;

namespace xe
{
	// the image file extension
	constexpr char IMG_VEC3[4] = { 'v','e','c','3' };  // RGB  decode jpeg
	constexpr char IMG_VEC4[4] = { 'v','e','c','4' };  // RGBA decode png
	constexpr char IMG_BOOL[4] = { 'b','o','o','l' };  // BOOL decode png

	std::unique_ptr<TestureEncodedData> TestureEncodedDataReader::GetTesture(const xeInt64 index)
	{
		auto output = std::make_unique<TestureEncodedData>();

		xeByte* decompress_data		= nullptr;
		xeUint64 file_size			= 0;
		const xeU8cstr* file_name	= GetAssetName(index);
		decompress_data				= GetAsset(index);

		// decompress failed
		if (decompress_data == nullptr)
		{
			return nullptr;
		}

		if (is4Extension(file_name, std::strlen(reinterpret_cast<const char*>(file_name)), IMG_VEC3))
		{
			output->encodesolution = xeColorChannel::RGB;
		}
		if (is4Extension(file_name, std::strlen(reinterpret_cast<const char*>(file_name)), IMG_VEC4))
		{
			output->encodesolution = xeColorChannel::RGBA;
		}
		if (is4Extension(file_name, std::strlen(reinterpret_cast<const char*>(file_name)), IMG_BOOL))
		{
			output->encodesolution = xeColorChannel::BOOL;
		}
		else
		{
			XE_WARNING_OUTPUT(std::format("<CLASS: TestureEncodedDataReader> File : \"{0}\" name is broken", reinterpret_cast<const char*>(file_name)).c_str());
			xeFree(decompress_data);
			return nullptr;
		}
		std::memcpy(output->name, file_name, 512);
		output->data = decompress_data;
		output->size = file_size;
		return output;
	}

	// the audio file extension
	constexpr char AUDIO_OGG8[4] = { 'o','g','g','\0' };  // decode ogg
	constexpr char AUDIO_FLAC[4] = { 'f','l','a','c' };   // decode flac

	std::unique_ptr<AudioEncodedData> AudioEncodedDataReader::GetAudio(const xeInt64 index)
	{
		auto output = std::make_unique<AudioEncodedData>();

		xeByte* decompress_data = nullptr;
		xeUint64 file_size = 0;
		const xeU8cstr* file_name = GetAssetName(index);
		decompress_data = GetAsset(index);

		if (decompress_data == nullptr)
		{
			return nullptr;
		}
		if (is4Extension(file_name, std::strlen(reinterpret_cast<const char*>(file_name)), AUDIO_OGG8))
		{
			output->solution = xeAudioCompressSolution::OGG;
		}
		if (is4Extension(file_name, std::strlen(reinterpret_cast<const char*>(file_name)), AUDIO_FLAC))
		{
			output->solution = xeAudioCompressSolution::FLAC;
		}
		else
		{
			// NAME BROKEN
			XE_WARNING_OUTPUT(std::format("<CLASS: AudioEncodedDataReader> File : \"{0}\" name is broken", reinterpret_cast<const char*>(file_name)).c_str());
			return nullptr;
		}
		std::memcpy(output->name, file_name, 512);
		output->data = decompress_data;
		output->size = file_size;
		output->cur_ptr = output->data;
		return output;
	}

}