#include "XeBasicAsset.h"
#include "decompress/decompress.h"

namespace xe
{
	constexpr char IMG_VEC3[4] = { 'v','e','c','3' };
	constexpr char IMG_VEC4[4] = { 'v','e','c','4' };
	constexpr char IMG_BOOL[4] = { 'b','o','o','l' };
	Testure* TestureReader::GetTesture(const int64_t index)
	{
		Testure* output = nullptr;
		byte_t* decompress_data = nullptr;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index);
		if (!decompress_data)
		{
			return nullptr;
		}
		if(Is4Extension(file_name, strlen(file_name), IMG_VEC3))
		{
			if (!DecompressJPEG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), IMG_VEC4))
		{
			if (!DecompressPNG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), IMG_BOOL))
		{
			if (!DecompressPNG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		else
		{
			XE_WARNING_OUTPUT(std::format("{0} FILE NAME IS BROKEN", file_name).c_str());
			return nullptr;
		}
	IMGDECOMPRESS_FAILED:
		return nullptr;
	}
	Testure* TestureReader::GetTesture(const char* name)
	{
		const int64_t index = GetDecompressedDataIndex(name);
		return GetTesture(index);
	}

	constexpr char AUDIO_OGG8[4] = { 'o','g','g','8' };
	constexpr char AUDIO_FLAC[4] = { 'f','l','a','c' };
	Audio* AudioReader::GetAudio(const int64_t index)
	{
		Audio* output = nullptr;
		byte_t* decompress_data = nullptr;
		const char* file_name = GetDecompressedDataName(index);
		decompress_data = GetDecompressedData(index);
		if (!decompress_data)
		{
			return nullptr;
		}
		if (Is4Extension(file_name, strlen(file_name), AUDIO_OGG8))
		{
			output->solution = xeAudioCompressSolution::OGG;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), AUDIO_FLAC))
		{
			output->solution = xeAudioCompressSolution::FLAC;
			return output;
		}
		// NAME BROKEN
		XE_WARNING_OUTPUT(std::format("{0} FILE NAME IS BROKEN", file_name).c_str());
		return nullptr;
	}
	Audio* AudioReader::GetAudio(const char* name)
	{
		const int64_t index = GetDecompressedDataIndex(name);
		return GetAudio(index);
	}
}