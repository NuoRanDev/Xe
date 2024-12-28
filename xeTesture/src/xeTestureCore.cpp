#include "xeTesture.h"

namespace xe
{
	Testure::Testure(uint32_t i_x, uint32_t i_y, xeColorChannel i_channel)
	{
		x = i_x; y = i_y; channel = i_channel; bit_depth = 8;
		testure_size = i_x * i_y * static_cast<uint32_t>(i_channel);
		pixel_data = new byte_t[testure_size];
	}
	Testure::~Testure()
	{
		delete[]pixel_data;
	}
	void Testure::GetlineStart(byte_line* dst)
	{
		for (uint32_t i = 0; i < y; i++)
		{
			dst[i] = pixel_data + i * GetLineSize();
		}
	}
}