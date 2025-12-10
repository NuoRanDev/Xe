#include "testure/xeTesture.hpp"

#include "log/xeLogOutput.hpp"

#include <cstring>

#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)

#else
#include "SDL3/SDL_render.h"
#endif // RENDER API

namespace xe
{
	bool Texture::load_image(Image& src_img)
	{
#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)

#else
		SDL_PixelFormat format;
		switch (src_img.get_format())
		{
		case IMG_FORMAT::GRAY8:
			format = SDL_PIXELFORMAT_RGB24;
			break;
		case IMG_FORMAT::GRAY16:
			format = SDL_PIXELFORMAT_RGB48;
			break;
		case IMG_FORMAT::GA8:
			format = SDL_PIXELFORMAT_RGBA32;
			break;
		case IMG_FORMAT::GA16:
			format = SDL_PIXELFORMAT_RGBA64;
			break;
		case IMG_FORMAT::RGB888:
			format = SDL_PIXELFORMAT_RGB24;
			break;
		case IMG_FORMAT::RGBA8888:
			format = SDL_PIXELFORMAT_RGBA32;
			break;
		case IMG_FORMAT::RGB161616:
			format = SDL_PIXELFORMAT_RGB48;
			break;
		case IMG_FORMAT::RGBA16161616:
			format = SDL_PIXELFORMAT_RGBA64;
			break;
		default:
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeTexture", "Unsupported image format");
			return false;
		}
		SDL_Surface* load_sdl_surafce = SDL_CreateSurface(src_img.get_width(), src_img.get_height(), format);
		auto render = std::any_cast<SDL_Renderer*>(bind_window->sdl_renderer);

		auto src_data= src_img.data();
		byte_t* dst_data = static_cast<byte_t*>(load_sdl_surafce->pixels);

		if (!is_gray(src_img.get_format()))
		{
			memcpy(dst_data, src_data, src_img.get_data_size());
		}
		//SDL_LoadBMP();
		else
		{
			if (has_alpha(src_img.get_format()))
			{
				ga_to_rgba(src_img.data(), reinterpret_cast<byte_t*>(load_sdl_surafce->pixels), src_img.get_width(), src_img.get_height(), get_format_channel_size(src_img.get_format()));
			}
			else
			{
				gray_to_rgb(src_img.data(), reinterpret_cast<byte_t*>(load_sdl_surafce->pixels), src_img.get_width(), src_img.get_height(), get_format_channel_size(src_img.get_format()));
			}
		}
		texture_obj = SDL_CreateTextureFromSurface(render, load_sdl_surafce);
		SDL_DestroySurface(load_sdl_surafce);
#endif // RENDER API
		return true;
	}

	bool Texture::draw(float dst_x, float dst_y, float dst_width, float dst_height)
	{
#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)
		return true;
#else
		auto src_tex = std::any_cast<SDL_Texture*>(texture_obj);
		auto render = std::any_cast<SDL_Renderer*>(bind_window->sdl_renderer);
		SDL_FRect dst_rect =
		{
			.x = static_cast<float>(dst_x),
			.y = static_cast<float>(dst_y),
			.w = static_cast<float>(dst_width),
			.h = static_cast<float>(dst_height)
		};
		return SDL_RenderTexture(render, src_tex, nullptr, &dst_rect);
#endif // RENDER API
	}

	Texture::~Texture()
	{

#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)

#else
		SDL_DestroyTexture(std::any_cast<SDL_Texture*>(texture_obj));
#endif // RENDER API
	}

} // namespace xe is end