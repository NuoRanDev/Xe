#include "decode/private_ImageFileReader.hpp"

// LIBJPEG
#include <csetjmp>

#include "libspng/spng.h"

#include "libjpeg/jpeglib.h"

namespace Jpeg
{
	struct JpegErrorMgr
	{
		jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	static void JpegErrorExit(j_common_ptr cinfo)
	{
		JpegErrorMgr* myerr = (JpegErrorMgr*)cinfo->err;
		(*cinfo->err->output_message) (cinfo);
		longjmp(myerr->setjmp_buffer, 1);
	}
}

namespace xe
{
	bool DecompressJPEG(Testure* img, byte_t* jpeg_buffer)
	{
		int width = 0;
		int height = 0;
		int64_t jpeg_size = 0;

		jpeg_decompress_struct cinfo;
		Jpeg::JpegErrorMgr jerr;

		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = Jpeg::JpegErrorExit;
		if (setjmp(jerr.setjmp_buffer))
		{
			jpeg_destroy_decompress(&cinfo);
			XE_WARNING_OUTPUT("MOD: <DecompressJPEG> Read data failed\n");
			return false;
		}

		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buffer, (unsigned long)jpeg_size);
		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress(&cinfo);

		width = cinfo.output_width;
		height = cinfo.output_height;

		img = new Testure(width, height, xeColorChannel::RGB);

		// Get line data point start and read image
		byte_line* img_data_lines = new byte_line[img->y];
		img->GetlineStart(img_data_lines);
		jpeg_read_scanlines(&cinfo, img_data_lines, img->y);
		delete[]img_data_lines;


		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		return true;
	}
	bool DecompressPNG(Testure* img, byte_t* png_buffer, size_t file_size)
	{
		uint8_t color_type = 0;
		uint32_t width = 0;
		uint32_t height = 0;
		spng_ihdr ihdr = { 0 };
		// need alloc memory
		size_t need_new_size = 0;
		Testure* output_mat = nullptr;
		//
		int32_t fmt = 0;
		int32_t ret = 0;

		spng_ctx* ctx = spng_ctx_new(0);
		if (ctx == nullptr)
		{
			XE_WARNING_OUTPUT("LIB: <Libspng> init context failed\n");
			return false;
		}

		ret = spng_set_png_buffer(ctx, png_buffer, file_size);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Load data failed\n");
			return false;
		}

		spng_get_ihdr(ctx, &ihdr);
		if (ret)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("LIB: <Libspng> Get header information data failed\n");
			return false;
		}

		// Xe studio output data is 8 bit depth
		if (ihdr.bit_depth != 8)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT(std::format("MOD: <DecompressPNG> Image data is broken. Image bit depth is {0}, XE support Bit depthis 8!", ihdr.bit_depth).c_str());
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
			output_mat = new Testure(width, height, xeColorChannel::RGBA);
			break;

		case SPNG_COLOR_TYPE_GRAYSCALE:
			spng_decoded_image_size(ctx, SPNG_FMT_G8, &need_new_size);
			fmt = SPNG_FMT_G8;
			output_mat = new Testure(width, height, xeColorChannel::BOOL);
			break;

		default:
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecompressPNG> Image data is broken. Not support this color format\n");
			return false;
		}

		// Get line data point start and read image
		if (need_new_size != output_mat->testure_size)
		{
			spng_ctx_free(ctx);
			XE_WARNING_OUTPUT("MOD: <DecompressPNG> Image data is broken. Alloc size failed\n");
			return false;
		}
		spng_decode_image(ctx, output_mat->pixel_data, need_new_size, fmt, 0);

		// Destory linspng struction
		spng_ctx_free(ctx);
		return true;
	}
}
