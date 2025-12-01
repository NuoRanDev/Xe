#include "window/xeWindow.hpp"

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.hpp"

#include <format>

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
	Window::Window()
	{
		is_draw = new std::mutex();
		current_draw_function_index = 0;
	}

	bool Window::create_window_context(int32_t w, int32_t h, xeString name, bool bordered) noexcept
	{
		init_render_api();
		sdl_window_contest = SDL_CreateWindow(name.c_str(), w, h, window_flags);
		if (!std::any_cast<SDL_Window*>(sdl_window_contest))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", (std::string("Failed to create window: ") + SDL_GetError()).c_str());
			return false;
		}
		bind_render_api_in_window();
		SDL_SetWindowPosition(std::any_cast<SDL_Window*>(sdl_window_contest), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(std::any_cast<SDL_Window*>(sdl_window_contest), bordered);
		command_map.create_empty(w, h);

		return true;
	}

	bool Window::draw_loop()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_contest);
		auto window_renderer = std::any_cast<SDL_Renderer*>(sdl_renderer);

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
			
			is_draw->lock();
			uint32_t next_draw_function_index;

			if (!current_draw_function_index) { next_draw_function_index = 1; }
			else next_draw_function_index = 0;

			while (!window_draw_functions[current_draw_function_index].empty())
			{
				auto draw_function = window_draw_functions[current_draw_function_index].front();
				if (!draw_function.stop_loop)
				{
					window_draw_functions[next_draw_function_index].push(draw_function);
				}
				window_draw_functions[current_draw_function_index].pop();
				draw_function.draw_function(this);
			}
			current_draw_function_index = next_draw_function_index;
			is_draw->unlock();

			SDL_RenderPresent(window_renderer);
		}
		return true;
	}

	void Window::submit_draw_command(std::function<bool(Window*)> draw_cmd, bool is_stop_loop)
	{
		is_draw->lock();
		window_draw_functions[current_draw_function_index].push({ draw_cmd, is_stop_loop });
		is_draw->unlock();
	}

	int32_t Window::get_width()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_contest);
		int32_t w;
		SDL_GetWindowSize(window, &w, nullptr);
		return w;
	}

	int32_t Window::get_height()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_contest);
		int32_t h;
		SDL_GetWindowSize(window, nullptr, &h);
		return h;
	}

	Window::~Window()
	{
		xe_delete_array(is_draw);
		auto window = std::any_cast<SDL_Window*>(sdl_window_contest);
#if defined(USE_OPENGL)
		(void)SDL_GL_DestroyContext(reinterpret_cast<SDL_GLContext>(context_opengl_instance));
#elif defined(USE_VULKAN)
#else
		SDL_DestroyRenderer(std::any_cast<SDL_Renderer*>(sdl_renderer));
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

	bool Window::bind_render_api_in_window()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_contest);
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