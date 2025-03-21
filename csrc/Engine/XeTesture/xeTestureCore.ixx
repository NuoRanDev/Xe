export module xe.xeTesture.xeTestureCore;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeAlloc;

namespace xe
{
	export class Testure
	{
	public:
		xeUint32		x, y;
		xeColorChannel	channel;
		xeUint32		bit_depth;
		xeByte			*pixel_data;
		xeInt64			testure_size;
		Testure() = default;

		~Testure()
		{
			xeFree(pixel_data);
		}

		void LoadData(xeUint32 i_x, xeUint32 i_y, xeColorChannel i_channel)
		{
			x = i_x; y = i_y; channel = i_channel; bit_depth = 8;
			testure_size = i_x * i_y * static_cast<xeUint32>(i_channel);
			pixel_data = xeMalloc<xeByte>(testure_size);
		}

		constexpr xeByte* GetData()
		{
			return pixel_data;
		}

		constexpr xeUint32 GetLineSize()
		{
			return x * static_cast<xeUint32>(channel) * (bit_depth / 8);
		}

		constexpr xeUint32 GetTestureSize()
		{
			return GetLineSize() * y;
		}
	};
}