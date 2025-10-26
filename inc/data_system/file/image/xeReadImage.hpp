#ifndef _XE_RWAD_IMAGE_HPP_
#define _XE_RWAD_IMAGE_HPP_

#include "type/xeOrdinals.hpp"
#include "image/xeImage.hpp"
#include "file/image/xeImgaeFile.hpp"

namespace xe
{

	bool read_memory_png_image(const ImageFile& file, Image& image) noexcept;

	bool read_memory_jpg_image(const ImageFile& file, Image& image) noexcept;
#if defined(USE_AVIF)
	bool read_memory_avif_image(const ImageFile& file, Image& image) noexcept;
#endif // USE_AVIF


} // namespace xe is end

#endif // _XE_RWAD_IMAGE_HPP_ IS EOF