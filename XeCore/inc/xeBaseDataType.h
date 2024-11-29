#ifndef _INC_XE_BASE_DATA_TYPE_H_
#define _INC_XE_BASE_DATA_TYPE_H_
#include "xeCoreConfig.h"

namespace xe
{
	enum class xeAudioCompressSolution :uint32_t
	{
		ERROR	= 0,

		FLAC	= 1,
		MP3		= 2,
		OGG		= 3
	};

	enum class xeColorChannel :uint32_t
	{
		GRAY	= 1,
		RGB		= 3,
		RGBA	= 4
	};

	// pxiel imgae
	struct Testure
	{
		uint32_t		x, y;
		xeColorChannel	channel;
		uint32_t		channel_byte_size = 1;
		byte_t*			pixel_data;
	};

	// Audio
	struct Audio
	{
		xeAudioCompressSolution solution;
		size_t _size;
		byte_t* data;
	};

	struct vec4_f32
	{
		float v1;
		float v2;
		float v3;
		float v4;
	};

	struct vec3_f32
	{
		float v1;
		float v2;
		float v3;
	};

	struct vec3_i32
	{
		int32_t v1;
		int32_t v2;
		int32_t v3;
	};

	struct vec4_i32
	{
		int32_t v1;
		int32_t v2;
		int32_t v3;
		int32_t v4;
	};
}

#endif // _INC_XE_BASE_DATA_TYPE_H_ IS EOF