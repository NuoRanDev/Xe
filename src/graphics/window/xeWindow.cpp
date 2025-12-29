#include "window/xeWindow.hpp"

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.hpp"

#include <format>

#if defined(USE_OPENGL)
constexpr SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL;
#elif defined(USE_VULKAN)
#include "graphicsAPI/vulkan/xeVkContext.hpp"
#include "SDL3/SDL_vulkan.h"
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

	bool Window::create_window_context(const char* exe_name, int32_t w, int32_t h, String name, bool bordered) noexcept
	{
		init_render_api(exe_name);
		sdl_window_context = SDL_CreateWindow(name.c_str(), w, h, window_flags);
		if (!std::any_cast<SDL_Window*>(sdl_window_context))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", (std::string("Failed to create window: ") + SDL_GetError()).c_str());
			return false;
		}
		bind_render_api_in_window();
		SDL_SetWindowPosition(std::any_cast<SDL_Window*>(sdl_window_context), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(std::any_cast<SDL_Window*>(sdl_window_context), bordered);
		command_map.create_empty(w, h);

		return true;
	}

	bool Window::draw_loop()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_context);
#if defined(USE_OPENGL)
#elif defined(USE_VULKAN)
		auto window_render = std::any_cast<VulkanContext*>(renderer_context);
#else
		auto window_renderer = std::any_cast<SDL_Renderer*>(sdl_renderer);
#endif // RENDER API

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
		int32_t w;
		SDL_GetWindowSize(std::any_cast<SDL_Window*>(sdl_window_context), &w, nullptr);
		return w;
	}

	int32_t Window::get_height()
	{
		int32_t h;
		SDL_GetWindowSize(std::any_cast<SDL_Window*>(sdl_window_context), nullptr, &h);
		return h;
	}

	Window::~Window()
	{
		xe_delete_array(is_draw);
		auto window = std::any_cast<SDL_Window*>(sdl_window_context);

#if defined(USE_OPENGL)
#elif defined(USE_VULKAN)
		auto vk_context = std::any_cast<VulkanContext*>(renderer_context);
		delete vk_context;
#else
		SDL_DestroyRenderer(std::any_cast<SDL_Renderer*>(sdl_renderer));
#endif // RENDER API
		SDL_DestroyWindow(window);
	}

	bool Window::init_render_api(const char* exe_name)
	{
#if defined(USE_OPENGL)

#elif defined(USE_VULKAN)
		VulkanContext* vk_context = new VulkanContext();
		uint32_t extension_count = 0;
		String cur_gpu_name;
		char const* const* extension = SDL_Vulkan_GetInstanceExtensions(&extension_count);
		
		if (!vk_context->init_vulkan_instance(extension, extension_count, exe_name))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : VULKAN", "Initialization vulkan is faild");
			return false; // Initialization failed
		}
		if (!(vk_context->link_physical_device()))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : VULKAN", "Not supported GPU");
			return false; // Initialization failed
		}
		const auto& gpu_list = vk_context->get_physical_device_list();
		for (auto& gpu : gpu_list)
		{
			if (gpu.get_device_name().find_start(u8"nvidia", 7) != -1)
			{
				cur_gpu_name = gpu.get_device_name();
				return true;
			}
		}
		cur_gpu_name = gpu_list[0].get_device_name();
		vk_context->pick_up_physical_device(cur_gpu_name);
		renderer_context = vk_context;
#else
#endif // RENDER API
		return true;
	}

	bool Window::bind_render_api_in_window()
	{
		auto window = std::any_cast<SDL_Window*>(sdl_window_context);
#if defined(USE_OPENGL)
#elif defined(USE_VULKAN)
		auto vk_context = std::any_cast<VulkanContext*>(renderer_context);
		VkSurfaceKHR vk_surface;
		SDL_Vulkan_CreateSurface(std::any_cast<SDL_Window*>(sdl_window_context), vk_context->vk_instance, nullptr, &vk_surface);
		vk_context->link_window_surface(vk_surface);
		float nu = 1.0f;
		vk_context->create_logical_device(&nu, 1);
#else
		renderer_context = SDL_CreateRenderer(window, NULL);;
#endif // RENDER API
		return true;
	}
} // namespace xe is end