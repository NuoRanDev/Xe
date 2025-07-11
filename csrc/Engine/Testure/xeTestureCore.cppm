export module xe.Testure.xeTestureCore;

import xe.Core.xeOrdinals;
import xe.Core.xeBaseDataType;
import xe.Core.xeAlloc;

namespace xe
{
	export class Testure
	{
	public:
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

		template<typename T>constexpr T* GetData()
		{
			return reinterpret_cast<T*>(pixel_data);
		}

		constexpr xeUint64 GetLineSize()
		{
			return x * static_cast<xeUint64>(channel) * (bit_depth / 8);
		}

		constexpr xeUint64 GetTestureSize()
		{
			return GetLineSize() * y;
		}

	public:

		xeUint32		x, y;
		xeColorChannel	channel;
		xeUint32		bit_depth;

	private:
		xeByte*			pixel_data;
		xeUint64		testure_size;
	};
}