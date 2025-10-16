#ifndef _XE_IMAGE_HPP_
#define _XE_IMAGE_HPP_

#include "type/xeOrdinals.hpp"
#include "memory/xeAlloc.hpp"

#include <functional>

namespace xe
{
	// pixel's channel and channel size
	enum class IMG_FORMAT
	{
		NONE			= 0,
		GRAY8,
		GRAY16,
		GA8,
		GA16,
		RGB888,
		RGBA8888,
		RGB161616,
		RGBA16161616,
	};

	constexpr size_t get_format_channel_size(IMG_FORMAT format) noexcept
	{
		switch(format)
		{
		case IMG_FORMAT::GRAY8:			return 1;
		case IMG_FORMAT::GRAY16:		return 2;
		case IMG_FORMAT::GA8:			return 2;
		case IMG_FORMAT::GA16:			return 4;
		case IMG_FORMAT::RGB888:		return 3;
		case IMG_FORMAT::RGBA8888:		return 4;
		case IMG_FORMAT::RGB161616:		return 6;
		case IMG_FORMAT::RGBA16161616:	return 8;
		}
	}

	constexpr size_t get_img_size(IMG_FORMAT format, size_t x, size_t y)
	{
		return get_format_channel_size(format) * x * y;
	}

	class Image
	{
	public:
		// check memory pool
		Image(std::function<byte_t* (size_t)>alloc_func = &xe_malloc<byte_t>)
		{
			alloc_data_memory	= alloc_func;
			img_data			= nullptr;
			data_size			= 0;
			format				= IMG_FORMAT::NONE;
			x = 0, y = 0;
		}

		void create_empty(IMG_FORMAT src_format, size_t src_x, size_t src_y) noexcept;

		[[nodiscard]] bool load_data(IMG_FORMAT src_format, const byte_t* src_data, size_t src_x, size_t src_y) noexcept;

		[[nodiscard]] bool load_data(byte_t* src_data, size_t src_size) noexcept;

		[[nodiscard]] const byte_t* const get_data(size_t& out_data_size) const noexcept;

		[[nodiscard]] const size_t get_width() const noexcept { return x; }

		[[nodiscard]] const size_t get_height() const noexcept { return y; }
		
		[[nodiscard]] const IMG_FORMAT get_format() const noexcept { return format; }
		
		[[nodiscard]] const size_t get_data_size() const noexcept { return data_size; }

		// important :not safe function !!!!!!!!
		[[nodiscard]] byte_t* data() { return img_data; }

		~Image() {};

	private:

		byte_t* img_data;

		size_t data_size;

		int64_t x, y;

		IMG_FORMAT format;

		// load data function
		std::function<byte_t* (size_t)> alloc_data_memory;
	};
} // namespace xe is end

#endif // _XE_IMAGE_HPP_ IS EOF