import xe.xeTesture.decode.private_Imgdecoder;

import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeCoreClrOutput;

import xe.xeTesture.xeTestureCore;

#include "spng.h"

namespace xe
{
	std::unique_ptr<Testure> DecodePNG(xeByte* png_buffer, xeSize file_size)
	{
		std::unique_ptr<Testure> img = std::make_unique<Testure>();
		xeByte color_type = 0;
		spng_ihdr ihdr = { 0 };
		// need alloc memory
		xeSize need_new_size = 0;
		//
		xeInt32 fmt = 0;
		xeInt32 ret = 0;

		spng_ctx* ctx = spng_ctx_new(0);
		if (ctx == nullptr)
		{
			XE_WARNING_OUTPUT("LIB: <Libspng> init context failed");
			return nullptr;
		}

		ret = spng_set_png_buffer(ctx, png_buffer, file_size);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Load data failed");
			return nullptr;
		}

		spng_get_ihdr(ctx, &ihdr);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Get header information data failed");
			return nullptr;
		}

		// Xe studio output data is 8 bit depth
		if (ihdr.bit_depth != 8)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(std::format("MOD: <DecodePNG> Image data is broken. Image bit depth is {0}, XE support Bit depthis 8!", ihdr.bit_depth).c_str());
			return nullptr;
		}

		// Get image bit depth and color type
		color_type = ihdr.color_type;

		// Get all pixel png is Big-endian 
		switch (color_type)
		{
		case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA:
			spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
			fmt = SPNG_FMT_RGBA8;
			img->LoadData(ihdr.width, ihdr.height, xeColorChannel::RGBA);
			break;

		case SPNG_COLOR_TYPE_GRAYSCALE:
			spng_decoded_image_size(ctx, SPNG_FMT_G8, &need_new_size);
			fmt = SPNG_FMT_G8;
			img->LoadData(ihdr.width, ihdr.height, xeColorChannel::BOOL);
			break;

		default:
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecodePNG> Image data is broken. Not support this color format");
			return nullptr;
		}

		// Get line data point start and read image
		if (need_new_size != img->testure_size)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecodePNG> Image data is broken. Alloc size failed");
			return nullptr;
		}
		spng_decode_image(ctx, img->pixel_data, need_new_size, fmt, 0);

		// Destory linspng struction
		spng_ctx_free(ctx);
		return img;
	}
}