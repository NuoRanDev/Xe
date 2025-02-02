#ifndef _INC_XE_TESTURE_CORE_HPP_
#define _INC_XE_TESTURE_CORE_HPP_

#include "xeTestureConfig.hpp"

namespace xe
{
	class XE_TESTURE_EXPORT_C_PLUS_PLUS_API Testure
	{
	public:
		uint32_t		x, y;
		xeColorChannel	channel;
		uint32_t		bit_depth;
		byte_t*			pixel_data;
		int64_t			testure_size;
		Testure() = default;
		Testure(uint32_t i_x, uint32_t i_y, xeColorChannel i_channel);
		~Testure();
		constexpr byte_t* GetData()
		{
			return pixel_data;
		}
		constexpr uint32_t GetLineSize()
		{
			return x * static_cast<uint32_t>(channel) * (bit_depth / 8);
		}
		constexpr uint32_t GetTestureSize()
		{
			return GetLineSize() * y;
		}
		void GetlineStart(byte_line* dst);
	};
}

#endif // IS _INC_XE_TESTURE_CORE_HPP_ EOF