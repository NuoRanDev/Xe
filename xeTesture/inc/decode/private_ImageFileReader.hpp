#ifndef _INC_PRIVATE_IMAGE_FILE_READER_HPP_
#define _INC_PRIVATE_IMAGE_FILE_READER_HPP_

#include "xeTestureConfig.hpp"
#include "xeTestureCore.hpp"

namespace xe
{
	// RGB channel use JPEG
	bool DecompressJPEG(Testure* img, byte_t* jpeg_buffer);
	// RGBA channel use PNG
	bool DecompressPNG(Testure* img, byte_t* png_buffer, size_t file_size);
}

#endif // _INC_PRIVATE_IMAGE_FILE_READER_HPP_ IS EOF