export module xeTesture.xeTestureCore;

import xeCore.xeOrdinals;
import xeCore.xeBaseDataType;
import xeCore.xeAlloc;

namespace xe
{
	class Testure
	{
	public:
		xeUint32		x, y;
		xeColorChannel	channel;
		uint32_t		bit_depth;
		xeByte			* pixel_data;
		xeInt64			testure_size;
		Testure() = default;

		Testure(uint32_t i_x, uint32_t i_y, xeColorChannel i_channel)
		{
			x = i_x; y = i_y; channel = i_channel; bit_depth = 8;
			testure_size = i_x * i_y * static_cast<uint32_t>(i_channel);
			pixel_data =  xeMalloc<xeByte>(testure_size);
		}

		~Testure()
		{
			xeFree(pixel_data);
		}

		constexpr xeByte* GetData()
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

		void GetlineStart(byte_line* dst)
		{
			for (uint32_t i = 0; i < y; i++)
			{
				dst[i] = pixel_data + i * GetLineSize();
			}
		}
	};
}