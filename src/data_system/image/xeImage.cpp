#include "image/xeImage.hpp"
#include "log/xeLogOutput.hpp"

namespace xe
{
	void Image::create_empty(IMG_FORMAT src_format, size_t src_x, size_t src_y) noexcept
	{
		x = src_x, y = src_y;
		format = src_format;
		data_size = get_img_size(format, x, y);
		img_data = alloc_data_memory(data_size);
	}

	bool Image::load_data(IMG_FORMAT src_format, const byte_t* src_data, size_t src_x, size_t src_y) noexcept
	{
		create_empty(src_format, src_x, src_y);
		img_data = alloc_data_memory(data_size);
		memcpy(img_data, src_data, data_size);
		return true;
	}

	bool Image::load_data(byte_t* src_data, size_t src_size) noexcept
	{
		if (src_data == nullptr || src_size == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Image data is empty");
			return false;
		}
		if (src_size != data_size)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Image data size is not match");
			return false;
		}
		memcpy(img_data, src_data, src_size);
		data_size = src_size;
		return true;
	}

	const byte_t* const Image::get_data(size_t& out_data_size) const noexcept
	{
		if (img_data == nullptr || data_size == 0)
		{
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeDataSystem",
				"Image data is empty");
			return nullptr;
		}
		out_data_size = data_size;
		return img_data;
	}
} // namespace xe is end