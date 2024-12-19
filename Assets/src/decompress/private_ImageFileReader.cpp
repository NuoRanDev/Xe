#include "decompress/private_ImageFileReader.h"

// LIBJPEG
#include <csetjmp>

#include "png.h"

#include "jpeglib.h"

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

namespace Png
{
	static void GetPngDataPtr(png_structp png_ptr, png_bytep png_data, png_size_t data_size)
	{
		png_data = static_cast<xe::byte_t*>(png_get_io_ptr(png_ptr));
	};
}

namespace xe
{
	bool DecompressJPEG(Testure* img, byte_t* jpeg_buffer)
	{
		int width = 0;
		int height = 0;
		int64_t jpeg_size = 0;
		uint64_t line_ofset = 0;
		byte_t* rgb_buffer = nullptr;

		jpeg_decompress_struct cinfo;
		Jpeg::JpegErrorMgr jerr;

		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = Jpeg::JpegErrorExit;
		if (setjmp(jerr.setjmp_buffer))
		{
			jpeg_destroy_decompress(&cinfo);
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

		// USE RGB COLOR SPACE
		rgb_buffer = img->GetData();
		line_ofset = img->GetLineSize();

		// Get line data point start and read image
		byte_line* img_data_lines = new byte_line[img->y];
		img->GetlineStart(img_data_lines);
		jpeg_read_scanlines(&cinfo, img_data_lines, img->y);
		delete[]img_data_lines;


		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		return true;
	}

	bool DecompressPNG(Testure* img, byte_t* png_buffer) 
	{
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr)
		{
			XE_WARNING_OUTPUT("png_ptr init false\n");
			return false;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			XE_WARNING_OUTPUT("info_ptr init false\n");
			return false;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			XE_WARNING_OUTPUT("Not png file\n");
			return false;
		}
		png_set_read_fn(png_ptr, png_buffer, Png::GetPngDataPtr);
		png_read_info(png_ptr, info_ptr);

		// Get image bit depth and color type
		
		png_uint_32 bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

		// Xe studio output data is 8 bit depth
		if (bit_depth != 8)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			XE_WARNING_OUTPUT(std::format("Image data is broken. Image bit depth is {0}, XE support Bit depthis 8!", bit_depth).c_str());
			return false;
		}


		// Get all pixel png is Big-endian 
		switch (color_type)
		{
		case PNG_COLOR_TYPE_RGB_ALPHA:
			img = new Testure(png_get_image_width(png_ptr, info_ptr), png_get_image_height(png_ptr, info_ptr), xeColorChannel::RGBA);
			break;

		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			img = new Testure(png_get_image_width(png_ptr, info_ptr), png_get_image_height(png_ptr, info_ptr), xeColorChannel::BOOL);
			break;

		default:
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			XE_WARNING_OUTPUT("Can\'t support this color format\n");
			return false;
		}

		// Get line data point start and read image
		byte_line* img_data_lines = new byte_line[img->y];
		img->GetlineStart(img_data_lines);
		png_read_image(png_ptr, img_data_lines);
		delete[]img_data_lines;

		// Destory linpng struction
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return true;
	}
}
