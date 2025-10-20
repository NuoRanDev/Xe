#include "file/image/xeReadImage.hpp"
#include "jpeglib.h"

#include "log/xeLogOutput.hpp"
#include "type/xeOrdinals.hpp"

#include <cstdint>
#include <format>

#include <setjmp.h>

struct libjpeg_addons {
	struct jpeg_error_mgr pub; /* "public" fields */
	jmp_buf setjmp_buffer; /* for return to caller */
};

void my_error_exit(j_common_ptr cinfo)
{
	libjpeg_addons* myerr = (libjpeg_addons*)cinfo->err;

	(*cinfo->err->output_message) (cinfo);

	longjmp(myerr->setjmp_buffer, 1);
}

namespace xe
{
	bool read_memory_jpg_image(const ImageFile& file, Image& img_out) noexcept
	{
		jpeg_decompress_struct cinfo;
		libjpeg_addons jerr;

		byte_t* image_data = nullptr;
		uint32_t line_size = 0;

		size_t mem_size = 0;
		auto mem_data = file.get_file_data(mem_size);

		auto cpub = &jerr.pub;

		cinfo.err = jpeg_std_error(cpub);
		jerr.pub.error_exit = my_error_exit;

		if (setjmp(jerr.setjmp_buffer))
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSystem : libjpeg", std::format("File:{0} reed jpeg failed!",file.c_file_name()).c_str());
			return false;
		}

		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, mem_data, static_cast<unsigned long>(mem_size));
		//read header
		jpeg_read_header(&cinfo, true);
		//start decompress
		jpeg_start_decompress(&cinfo);

		// line size
		uint32_t stride = cinfo.output_width * cinfo.output_components;

		// read parameters
		switch (cinfo.out_color_space)
		{
		case JCS_UNKNOWN:
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSystem: libjpeg", std::format("File:{0} unknown jpeg type\n",file.c_file_name()).c_str());
			return false;

		case JCS_GRAYSCALE:
			img_out.create_empty(IMG_FORMAT::GRAY8, cinfo.image_width, cinfo.image_height);
			break;

		case JCS_RGB:
			img_out.create_empty(IMG_FORMAT::RGB888, cinfo.image_width, cinfo.image_height);
			break;

		default:
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "DataSystem: libjpeg", std::format("File:{0} unknown jpeg type\n",file.c_file_name()).c_str());
			return false;
		}

		// get pixel lines
		image_data = img_out.unsafe_data();
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, &image_data, 1);
			image_data += stride;
		}

		//finished
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		return true;

	}
} // namespace xe is end

