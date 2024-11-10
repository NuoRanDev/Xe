#include "decompress/private/private_ImageFileReader.h"

// LIBJPEG
#include "jpeglib.h"
namespace xe
{
	constexpr uint64_t RGB_PIXEL_OFFSET = 3;

	int DecompressJPEG(Testure* img, byte_t* jpeg_buffer)
	{
		int width = 0;
		int height = 0;
		int64_t jpeg_size = 0;
		uint64_t line_ofset = 0;
		byte_t* rgb_buffer = nullptr;

		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;

		cinfo.err = jpeg_std_error(&jerr);

		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buffer, (unsigned long)jpeg_size);
		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress(&cinfo);

		width = cinfo.output_width;
		height = cinfo.output_height;

		img->x = width;
		img->y = height;
		img->channel = RGB_PIXEL_OFFSET;
		img->channel_size = 8;

		// USE RGB COLOR SPACE
		rgb_buffer = new byte_t[width * height * RGB_PIXEL_OFFSET];
		line_ofset = width * RGB_PIXEL_OFFSET;

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

		return 0;
	}
}
