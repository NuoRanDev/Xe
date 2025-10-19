#include "testure/xeTesture.hpp"

#include "log/xeLogOutput.hpp"

#include <cstring>

#include <iostream>

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
		auto src_data= src_img.data();
		auto dst_data = load_sdl_surafce->pixels;
		std::cout<<(uint32_t)(src_img.data()[21]);


		if (!is_gray(src_img.get_format()))
		{
			printf("%llu %llu %llu\n", src_img.get_width(), src_img.get_height(), src_img.get_data_size());
			printf(" %d %d %d\n", load_sdl_surafce->w, load_sdl_surafce->h, load_sdl_surafce->w * load_sdl_surafce->h * 3);
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
		SDL_DestroySurface(load_sdl_surafce);
		SDL_CreateTextureFromSurface(std::any_cast<SDL_Renderer*>(bind_window.sdl_renderer), load_sdl_surafce);
		texture_obj = std::move(load_sdl_surafce);
#endif // RENDER API
		return true;
	}

	bool Texture::draw(int32_t dst_x, int32_t dst_y, size_t dst_width, size_t dst_height)
	{
#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)

#else
		auto src_tex = std::any_cast<SDL_Texture*>(texture_obj);
		SDL_FRect dst_rect =
		{
			.x = static_cast<float>(dst_x) / bind_window.get_width(),
			.y = static_cast<float>(dst_y) / bind_window.get_height(),
			.w = static_cast<float>(dst_width) / src_tex->w,
			.h = static_cast<float>(dst_height) / src_tex->h
		};
		return SDL_RenderTexture(std::any_cast<SDL_Renderer*>(bind_window.sdl_renderer),
			std::any_cast<SDL_Texture*>(texture_obj), nullptr, &dst_rect);
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