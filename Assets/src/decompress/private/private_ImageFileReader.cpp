#include "decompress/private/private_ImageFileReader.h"

// LIBJPEG
#include <csetjmp>

#include "jpeglib.h"

namespace xe
{
	struct my_error_mgr
	{
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	typedef struct my_error_mgr* my_error_ptr;
	static void my_error_exit(j_common_ptr cinfo)
	{
		my_error_ptr myerr = (my_error_ptr)cinfo->err;
		(*cinfo->err->output_message) (cinfo);
		longjmp(myerr->setjmp_buffer, 1);
	}

	bool DecompressJPEG(Testure* img, byte_t* jpeg_buffer, xeColorChannel out_channel)
	{
		int width = 0;
		int height = 0;
		int64_t jpeg_size = 0;
		uint64_t line_ofset = 0;
		byte_t* rgb_buffer = nullptr;

		jpeg_decompress_struct cinfo;
		my_error_mgr jerr;

		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
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

		img->x = width;
		img->y = height;
		img->channel = out_channel;

		// USE RGB COLOR SPACE
		rgb_buffer = new byte_t[width * height * static_cast<int>(out_channel)];
		line_ofset = width * static_cast<int>(out_channel);

		byte_line* ppixel_line_data = nullptr;

		img->pixel_data = rgb_buffer;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			ppixel_line_data = &rgb_buffer;
			jpeg_read_scanlines(&cinfo, ppixel_line_data, 1);
			rgb_buffer = rgb_buffer + line_ofset;
		}

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		return true;
	}
}
