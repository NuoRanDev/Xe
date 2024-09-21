#ifndef _INC_LAZY_IMG_CONFIG_H_
#define _INC_LAZY_IMG_CONFIG_H_

// std library
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif // _WIN32

#define LAZY_API_EXPORT __declspec(dllexport)

namespace lazyimg
{
	// define byte
	using byte = uint8_t;
}

// This is pixel format

namespace pformat
{
	enum COLOR_FORMAT :lazyimg::byte
	{
		U8_GRAYSCALE	= 0,
		U16_GRAYSCALE	= 1,
		U8_RGB			= 2,
		U8_BGR			= 3,
		U8_RGBA			= 4,
		U16_RGB			= 5,
		U16_BGR			= 6,
		U16_RGBA		= 7,
		U8_ABGR			= 8,
		U16_ABGR		= 9,
		U8_YUV			= 10
	};

	// This define the image's pixel size

	enum ByteDataSize :lazyimg::byte
	{
		U8	= 0b0001,
		U16 = 0b0010,
		U24 = 0b0100,
		U32 = 0b1000
	};

	// This define the image's color channel number

	enum ColorByteDeepSize :lazyimg::byte
	{
		C1 = 0b0001,
		C2 = 0b0010,
		C3 = 0b0100,
		C4 = 0b1000
	};
}

#endif // _INC_LAZY_IMG_CONFIG_H_ IS EOF