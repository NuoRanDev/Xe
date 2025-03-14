export module xeTesture.decode.private_ImageFileReader;

import std;

import "spng.h";

import xeCore.xeOrdinals;
import xeTesture.xeTestureCore;
import xeCore.xeCoreClrOutput;

namespace xe
{
	export bool DecompressPNG(std::unique_ptr<Testure> img, xeByte* png_buffer, xeSize file_size)
	{
		xeByte color_type = 0;
		xeUint32 width = 0;
		xeUint32 height = 0;
		spng_ihdr ihdr = { 0 };
		// need alloc memory
		xeSize need_new_size = 0;
		//
		xeInt32 fmt = 0;
		xeInt32 ret = 0;

		spng_ctx* ctx = spng_ctx_new(0);
		if (ctx == nullptr)
		{
			XE_WARNING_OUTPUT("LIB: <Libspng> init context failed\n");
			img = nullptr;
			return false;
		}

		ret = spng_set_png_buffer(ctx, png_buffer, file_size);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Load data failed\n");
			img = nullptr;
			return false;
		}

		spng_get_ihdr(ctx, &ihdr);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Get header information data failed\n");
			img = nullptr;
			return false;
		}

		// Xe studio output data is 8 bit depth
		if (ihdr.bit_depth != 8)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(std::format("MOD: <DecompressPNG> Image data is broken. Image bit depth is {0}, XE support Bit depthis 8!", ihdr.bit_depth).c_str());
			img = nullptr;
			return false;
		}
		color_type = ihdr.color_type;

		// Get image bit depth and color type
		color_type = ihdr.color_type;

		// Get all pixel png is Big-endian 
		switch (color_type)
		{
		case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA:
			spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
			fmt = SPNG_FMT_RGBA8;
			img = std::make_unique<Testure>(width, height, xeColorChannel::RGBA);
			break;

		case SPNG_COLOR_TYPE_GRAYSCALE:
			spng_decoded_image_size(ctx, SPNG_FMT_G8, &need_new_size);
			fmt = SPNG_FMT_G8;
			img = std::make_unique<Testure>(width, height, xeColorChannel::BOOL);
			break;

		default:
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecompressPNG> Image data is broken. Not support this color format\n");
			img = nullptr;
			return false;
		}

		// Get line data point start and read image
		if (need_new_size != img->testure_size)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecompressPNG> Image data is broken. Alloc size failed\n");
			img = nullptr;
			return false;
		}
		spng_decode_image(ctx, img->pixel_data, need_new_size, fmt, 0);

		// Destory linspng struction
		spng_ctx_free(ctx);
		return true;
	}
}