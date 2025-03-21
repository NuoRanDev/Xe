import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeCoreClrOutput;

import xe.AssetIO.xeBasicAsset;
import xe.AssetIO.xeBasicData;

namespace xe
{
	// the image file extension
	constexpr char IMG_VEC3[4] = { 'v','e','c','3' };  // RGB  decode jpeg
	constexpr char IMG_VEC4[4] = { 'v','e','c','4' };  // RGBA decode png
	constexpr char IMG_BOOL[4] = { 'b','o','o','l' };  // BOOL decode png
	std::unique_ptr<TestureFile> TestureFileReader::GetTesture(const xeInt64 index)
	{
		std::unique_ptr<TestureFile> output;
		xeByte* decompress_data = nullptr;
		xeUint64 file_size = 0;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index, file_size);
		// decompress failed
		if (decompress_data == nullptr)
		{
			return nullptr;
		}
		if (is4Extension(file_name, std::strlen(file_name), IMG_VEC3))
		{
			output->encodesolution = xeColorChannel::RGB;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (is4Extension(file_name, std::strlen(file_name), IMG_VEC4))
		{
			output->encodesolution = xeColorChannel::RGBA;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (is4Extension(file_name, std::strlen(file_name), IMG_BOOL))
		{
			output->encodesolution = xeColorChannel::BOOL;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		else
		{
			XE_WARNING_OUTPUT(std::format("{0} FILE NAME IS BROKEN", file_name).c_str());
			return nullptr;
		}
	}
	std::unique_ptr<TestureFile> TestureFileReader::GetTesture(const char* name)
	{
		const xeInt64 index = GetDecompressedDataIndex(name);
		return GetTesture(index);
	}

	// the audio file extension
	constexpr char AUDIO_OGG8[4] = { 'o','g','g','8' };  // 8bit  decode ogg
	constexpr char AUDIO_FLAC[4] = { 'f','l','a','c' };  // 16bit decode flac
	std::unique_ptr<AudioFile> AudioReader::GetAudio(const xeInt64 index)
	{
		std::unique_ptr<AudioFile> output ;
		xeByte* decompress_data = nullptr;
		xeUint64 file_size = 0;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index, file_size);
		if (!decompress_data)
		{
			return nullptr;
		}
		if (is4Extension(file_name, std::strlen(file_name), AUDIO_OGG8))
		{
			output->solution = xeAudioCompressSolution::OGG;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (is4Extension(file_name, std::strlen(file_name), AUDIO_FLAC))
		{
			output->solution = xeAudioCompressSolution::FLAC;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		// NAME BROKEN
		XE_WARNING_OUTPUT(std::format("{0} FILE NAME IS BROKEN", file_name).c_str());
		return nullptr;
	}
	std::unique_ptr<AudioFile> AudioReader::GetAudio(const char* name)
	{
		const xeInt64 index = GetDecompressedDataIndex(name);
		return GetAudio(index);
	}
}