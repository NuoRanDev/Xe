#include "xeWindow.h"

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.h"

#include <format>

// SDL Window Struction
SDL_Window* void_to_sdlwindow(void* v_src) noexcept { return reinterpret_cast<SDL_Window*>(v_src); }
void* void_to_sdlwindow(SDL_Window* w_src) noexcept { return reinterpret_cast<void*>(w_src); }

#if defined(USE_OPENGL)
#include "SDL3/SDL_vulkan.h"
constexpr SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL;
#elif defined(USE_VULKAN)
constexpr SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN;
#else
#include "SDL3/SDL_render.h"
constexpr SDL_WindowFlags window_flags = 0;
#endif // CHOOSE RENDER API

namespace xe
{
	bool Window::create_window_context(int32_t w, int32_t h, xeString name, bool bordered) noexcept
	{
		init_render_api();
		sdl_window_contest = reinterpret_cast<void*>(SDL_CreateWindow(name.c_str(), w, h, window_flags));
		if (!sdl_window_contest)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", (std::string("Failed to create window: ") + SDL_GetError()).c_str());
			return false;
		}
		bind_reder_api_in_window();
		SDL_SetWindowPosition(reinterpret_cast<SDL_Window*>(sdl_window_contest), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(reinterpret_cast<SDL_Window*>(sdl_window_contest), bordered);

		return true;
	}

	bool Window::main_loop()
	{
		auto window = void_to_sdlwindow(sdl_window_contest);
		auto window_renderer = reinterpret_cast<SDL_Renderer*>(sdl_renderer);

		SDL_Event event;
		bool loopShouldStop = false;

		while (!loopShouldStop)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					loopShouldStop = true;
					break;
				}
			}

			SDL_RenderClear(window_renderer);
			SDL_RenderPresent(window_renderer);
		}
		return true;
	}

	Window::~Window()
	{
		auto window = void_to_sdlwindow(sdl_window_contest);
#if defined(USE_OPENGL)
		(void)SDL_GL_DestroyContext(reinterpret_cast<SDL_GLContext>(context_opengl_instance));
#elif defined(USE_VULKAN)
#else
		SDL_DestroyRenderer(reinterpret_cast<SDL_Renderer*>(sdl_renderer));
#endif // RENDER API
		SDL_DestroyWindow(window);
	}

	bool Window::init_render_api()
	{
#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)
		Uint32 count_instance_extensions;

		const char* const* sdl_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&count_instance_extensions);

		if (sdl_instance_extensions == nullptr)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", std::format("Failed to get the number of required instance extensions: {0}\n", SDL_GetError()).c_str());
			return false;
		}

		if (!window_vulkan_instance.SetVulkanInstanceContext(sdl_instance_extensions, count_instance_extensions, "XE_SDL3_VK"))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", "Failed to setup vulkan instance context");
			return false;
		}

#else
#endif // RENDER API
		return true;
	}

	bool Window::bind_reder_api_in_window()
	{
		auto window = void_to_sdlwindow(sdl_window_contest);
#if defined(USE_OPENGL)
		context_opengl_instance = reinterpret_cast<void*>(SDL_GL_CreateContext(window));
#elif defined(USE_VULKAN)
		if (!SDL_Vulkan_CreateSurface(reinterpret_cast<SDL_Window*>(window), window_vulkan_instance.GetInstance(), nullptr, window_vulkan_instance.GetSurface()))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", std::format("Failed to create vulkan surface: {0}\n", SDL_GetError()).c_str());
			return false;
		}
#else
		sdl_renderer = SDL_CreateRenderer(window, NULL);;
#endif // RENDER API
		return true;
	}
} // namespace xe is end