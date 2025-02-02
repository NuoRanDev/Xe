#include "xeBasicAsset.hpp"
#include "decompress/decompress.hpp"

namespace xe
{
	// the image file extension
	constexpr char IMG_VEC3[4] = { 'v','e','c','3' };  // RGB  decode jpeg
	constexpr char IMG_VEC4[4] = { 'v','e','c','4' };  // RGBA decode png
	constexpr char IMG_BOOL[4] = { 'b','o','o','l' };  // BOOL decode png
	TestureFile* TestureFileReader::GetTesture(const int64_t index)
	{
		TestureFile* output = nullptr;
		byte_t* decompress_data = nullptr;
		uint64_t file_size = 0;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index, file_size);
		// decompress failed
		if (decompress_data == nullptr)
		{
			return nullptr;
		}
		if(Is4Extension(file_name, strlen(file_name), IMG_VEC3))
		{
			output->encodesolution = xeColorChannel::RGB;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), IMG_VEC4))
		{
			output->encodesolution = xeColorChannel::RGBA;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), IMG_BOOL))
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
	TestureFile* TestureFileReader::GetTesture(const char* name)
	{
		const int64_t index = GetDecompressedDataIndex(name);
		return GetTesture(index);
	}

	// the audio file extension
	constexpr char AUDIO_OGG8[4] = { 'o','g','g','8' };  // 8bit  decode ogg
	constexpr char AUDIO_FLAC[4] = { 'f','l','a','c' };  // 16bit decode flac
	AudioFile* AudioReader::GetAudio(const int64_t index)
	{
		AudioFile* output = nullptr;
		byte_t* decompress_data = nullptr;
		uint64_t file_size = 0;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index, file_size);
		if (!decompress_data)
		{
			return nullptr;
		}
		if (Is4Extension(file_name, strlen(file_name), AUDIO_OGG8))
		{
			output->solution = xeAudioCompressSolution::OGG;
			output->data = decompress_data;
			output->_size = file_size;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), AUDIO_FLAC))
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
	AudioFile* AudioReader::GetAudio(const char* name)
	{
		const int64_t index = GetDecompressedDataIndex(name);
		return GetAudio(index);
	}
}