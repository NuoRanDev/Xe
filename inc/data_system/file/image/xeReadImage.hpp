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
	bool read_memory_texture_image(const ImageFile& file, Image& img_out) noexcept
	{
		size_t file_size;
		const byte_t* file_data = file.get_file_data(file_size);
		int64_t head_ofs = sizeof(IMG_FORMAT) + sizeof(int32_t) * 2;

		img_out.create_empty(*(reinterpret_cast<const IMG_FORMAT*>(file_data)),
			*(reinterpret_cast<const int32_t*>(file_data + sizeof(IMG_FORMAT))),
			*(reinterpret_cast<const int32_t*>(file_data + sizeof(IMG_FORMAT) + sizeof(int32_t))));
		std::memcpy(img_out.unsafe_data(), file_data + head_ofs, file_size - head_ofs);
		return true;
	}

} // namespace xe is end

#endif // _XE_RWAD_IMAGE_HPP_ IS EOF