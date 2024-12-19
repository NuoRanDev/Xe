#include "XeBasicAsset.h"
#include "decompress/decompress.h"

namespace xe
{
	constexpr char VEC3[4] = { 'v','e','c','3' };
	constexpr char VEC4[4] = { 'v','e','c','4' };
	constexpr char BOOL[4] = { 'b','o','o','l' };
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
		if(Is4Extension(file_name, strlen(file_name), VEC3))
		{
			if (!DecompressJPEG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), VEC4))
		{
			if (!DecompressPNG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		if (Is4Extension(file_name, strlen(file_name), BOOL))
		{
			if (!DecompressPNG(output, decompress_data))goto IMGDECOMPRESS_FAILED;
			return output;
		}
		else
		{
			XE_WARNING_OUTPUT(std::format("CAN'T SUPPORTED THIS FILE: {0}", file_name).c_str());
		}
	IMGDECOMPRESS_FAILED:
		return nullptr;
	}
	bool TestureReader::DecompressFunction(byte_t* compressed_data, int64_t compressed_size, byte_t* not_compressed_data, int64_t not_compressed_size)
	{
		if(!DecompressLZMA(compressed_data, not_compressed_data, compressed_size, not_compressed_size))
		{
			XE_ERROR_OUTPUT("decompress faild!\n");
			return false;
		}
		return true;
	}
}