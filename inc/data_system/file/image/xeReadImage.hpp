#ifndef _XE_RWAD_IMAGE_HPP_
#define _XE_RWAD_IMAGE_HPP_

#include "type/xeOrdinals.hpp"
#include "image/xeImage.hpp"
#include "file/image/xeImgaeFile.hpp"

namespace xe
{
#if defined(USE_PNG)
	bool read_memory_png_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_PNG
#if defined(USE_JPEG)
	bool read_memory_jpg_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_JPEG
#if defined(USE_AVIF)
	bool read_memory_avif_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_AVIF
	bool read_memory_ppm_image(const ImageFile& file, Image& img_out) noexcept;
} // namespace xe is end

#endif // _XE_RWAD_IMAGE_HPP_ IS EOF