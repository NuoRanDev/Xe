#ifndef _XE_IMAGE_HPP_
#define _XE_IMAGE_HPP_

#include "type/xeOrdinals.hpp"
#include "memory/xeAlloc.hpp"

#include <functional>

namespace xe
{
	// pixel's channel and channel size
	enum class IMG_FORMAT :uint32_t
	{
		NONE			= 0,
		GRAY8,
		GA8,
		GRAY16,
		GA16,
		RGB888,
		RGBA8888,
		RGB161616,
		RGBA16161616
	};

	constexpr bool is_gray(IMG_FORMAT foramt)
	{
		return static_cast<uint32_t>(foramt) < static_cast<uint32_t>(IMG_FORMAT::RGB888);
	}

	constexpr bool has_alpha(IMG_FORMAT format)
	{
		auto fmt_value = static_cast<uint32_t>(format);
		return fmt_value ^ 0x00'00'00'01u;
	}

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
		default:						return 0;
		}
	}

	constexpr int64_t get_img_size(IMG_FORMAT format, int32_t x, int32_t y)
	{
		return get_format_channel_size(format) * x * y;
	}

	void gray_to_rgb(const byte_t* src_data, byte_t* out_data, int32_t x, int32_t y, int64_t channel_size) noexcept;

	void ga_to_rgba(const byte_t* src_data, byte_t* out_data, int32_t x, int32_t y, int64_t channel_size) noexcept;

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

		Image(const Image&) noexcept = default;

		Image& operator=(const Image&) noexcept = default;

		Image& operator=(Image&&) noexcept = default;

		void create_empty(IMG_FORMAT src_format, int32_t src_x, int32_t src_y) noexcept;

		[[nodiscard]] bool load_data(IMG_FORMAT src_format, const byte_t* src_data, int32_t src_x, int32_t src_y) noexcept;

		[[nodiscard]] bool load_data(byte_t* src_data, int32_t src_size) noexcept;

		[[nodiscard]] const byte_t* const get_data(int64_t& out_data_size) const noexcept;

		[[nodiscard]] const int32_t get_width() const noexcept { return x; }

		[[nodiscard]] const int32_t get_height() const noexcept { return y; }
		
		[[nodiscard]] const IMG_FORMAT get_format() const noexcept { return format; }
		
		[[nodiscard]] const size_t get_data_size() const noexcept { return data_size; }

		// important :not safe function !!!!!!!!
		[[nodiscard]] byte_t* unsafe_data() noexcept;

		[[nodiscard]] const byte_t* data() const noexcept { return img_data; }

		~Image() 
		{ 
			xe_free(img_data); 
		}

	private:

		int64_t data_size;

		byte_t* img_data;

		int32_t x, y;

		IMG_FORMAT format;

		// load data function
		std::function<byte_t* (size_t)> alloc_data_memory;
	};
} // namespace xe is end

#endif // _XE_IMAGE_HPP_ IS EOF