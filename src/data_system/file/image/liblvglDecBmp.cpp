
#include "log/xeLogOutput.hpp"

#include <cstring>
#include <format>
#include "file/image/xeReadImage.hpp"

namespace xe
{
	static bool decoder_info(const ImageFile& file, Image& image)
	{
		// bmp color space
		uint16_t bpp;
		// bmp 
		uint32_t w;
		uint32_t h;
		// bmp header
		byte_t headers[54];

		std::memcpy(headers, file.get_file_data(0), 54);

		if (0x42 != headers[0] || 0x4d != headers[1])
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeDataSystem", "NOT BMP FILE");
			return false;
		}


		std::memcpy(&w, headers + 18, 4);
		std::memcpy(&h, headers + 22, 4);
		std::memcpy(&bpp, headers + 28, 2);
		switch (bpp) {
		case 16:
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeDataSystem", std::format("Not supported bpp: {0}", bpp).c_str());
			return false;
		case 24:
			image.create_empty(IMG_FORMAT::RGB_U888, w, h);
			break;
		case 32:
			image.create_empty(IMG_FORMAT::RGBA_U8888, w, h); //LV_COLOR_FORMAT_ARGB8888
			break;
		default:
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeDataSystem", std::format("Not supported bpp: {0}", bpp).c_str());
			return false;
		}
		return true;
	}
} // namespace xe is end
