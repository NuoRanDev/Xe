#include "file/image/xeReadImage.hpp"

// Thired-party library: libspng
#include "spng.h"

#include "log/xeLogOutput.hpp"
#include "type/xeOrdinals.hpp"

#include <cstdint>
#include <format>

namespace xe
{
	bool read_memory_png_image(const ImageFile& file, Image& img_out) noexcept
	{
		byte_t color_type = 0;
		spng_ihdr ihdr = { 0 };

		// need alloc memory
		size_t need_new_size = 0;

		int64_t x, y;
		//
		int32_t fmt = 0;
		int32_t ret = 0;

		size_t file_size;
		const byte_t* png_buffer;
		Image img;

		IMG_FORMAT chaanel_format;

		// Create image object
		png_buffer = file.get_file_data(file_size);
		if (png_buffer == nullptr || file_size == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspngAddons", "Image file data is empty");
			return false;
		}

		spng_ctx* ctx = spng_ctx_new(0);
		if (ctx == nullptr)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspngAddons", "Init context failed");
			return false;
		}

		ret = spng_set_png_buffer(ctx, png_buffer, file_size);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspngAddons", "Load data failed");
			return false;
		}

		spng_get_ihdr(ctx, &ihdr);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspngAddons", "Get header information data failed");
			return false;
		}

		// Xe studio output data is 8 bit depth
		if (ihdr.bit_depth != 8 and ihdr.bit_depth != 16)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspngAddons",
				std::format("Image data is broken. Image bit depth is {0}, XE support Bit depthis 8 and 16!", 
				ihdr.bit_depth).c_str());
			return false;
		}

		// Get image bit depth and color type
		color_type = ihdr.color_type;

		// Get all pixel png is Big-endian 
		switch (color_type)
		{
		case SPNG_COLOR_TYPE_GRAYSCALE:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_G8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA8888;
			}
			// if (ihdr.bit_depth == 16) 16bit gray is not support, what's fucking crazy man use it!
			else goto COLOR_NOT_SUPPORT;
			break;
		case SPNG_COLOR_TYPE_TRUECOLOR:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &need_new_size);
				fmt						= SPNG_FMT_RGB8;
				chaanel_format			= IMG_FORMAT::RGB888;
			}
			else goto COLOR_NOT_SUPPORT;
			break;
			// The fucking old color picture
		case SPNG_COLOR_TYPE_INDEXED:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA8888;
			}
			if (ihdr.bit_depth == 16)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA16, &need_new_size);
				fmt						= SPNG_FMT_RGBA16;
				chaanel_format			= IMG_FORMAT::RGBA16161616;
			}
			else goto COLOR_NOT_SUPPORT;
			break;
			// I think this format is used less, but my penis master thinks that can hide some sexy pictures....
		case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA:
			if (ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_GA8, &need_new_size);
				fmt						= SPNG_FMT_GA8;
				chaanel_format			= IMG_FORMAT::GA8;
			}
			if (ihdr.bit_depth == 16)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_GA16, &need_new_size);
				fmt						= SPNG_FMT_GA16;
				chaanel_format			= IMG_FORMAT::GA16;
			}
			else goto COLOR_NOT_SUPPORT;
			break;
		case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA8888;
			}
			if (ihdr.bit_depth == 16) 
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA16, &need_new_size);
				fmt						= SPNG_FMT_RGBA16;
				chaanel_format			= IMG_FORMAT::RGBA16161616;
			}
			else goto COLOR_NOT_SUPPORT;
			break;
		default:
			goto COLOR_NOT_SUPPORT;
		}

		img.create_empty(chaanel_format, ihdr.width, ihdr.height);
		
		// Get line data point start and read image
		if (need_new_size != img.get_data_size())
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspng", "Image data is broken or alloc size failed");
			return false;
		}
		spng_decode_image(ctx, img.data(), need_new_size, fmt, 0);

		// Destory linspng struction
		spng_ctx_free(ctx);
		img_out = std::move(img);
		return true;
	COLOR_NOT_SUPPORT:
		spng_ctx_free(ctx);
		XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSyatem : libspng", "Image data is broken.Not support this color format");
		return false;
	}
}