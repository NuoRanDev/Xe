#include "window/xeWindow.hpp"

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.hpp"

#include <format>

#include "SDL3/SDL_vulkan.h"

#include "graphicsAPI/xeVkContext.hpp"

constexpr SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN;

namespace xe
{
	Window::Window()
	{
		queue_count = DEFAULT_QUEUE_COUNT;
		queue_priorities = DEFAULT_QUEUE_PRIORITIES;
	}

	bool Window::create_window_context(const utf8_t* title, int32_t w, int32_t h, bool bordered) noexcept
	{
		VkSurfaceKHR window_surface;
		vulkan::VulkanContext *vk_context = new vulkan::VulkanContext();
		SDL_Window* window;
		
		// create vulkan instance
		uint32_t exetension_count = 0;
		char const* const* exetensions = SDL_Vulkan_GetInstanceExtensions(&exetension_count);
		if (exetensions == nullptr)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeWindow : SDL3", "Can't find vulkan exetrnsions");
			return false;
		}

		if(!vk_context->init_vulkan_instance(exetensions, exetension_count, title))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeWindow", "init vulkan instance failed");
			return false;
		}
		if(!vk_context->find_physical_device(want_extension_properties))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeWindow", "Can't find support vulkan device");
			return false;
		}

		// show window
		window = SDL_CreateWindow(reinterpret_cast<const char*>(title), w, h, window_flags);
		if (!window)
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeWindow : SDL3", (std::string("Failed to create window: ") + SDL_GetError()).c_str());
			return false;
		}
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(window, bordered);

		// link window surface to vulkan swap chain
		if (!SDL_Vulkan_CreateSurface(window, vk_context->instance, nullptr, &window_surface))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeWindow : SDL3", (std::string("Failed to create vulkan surface: ") + SDL_GetError()).c_str());
			return false;
		}
		vk_context->get_cur_window_surface(window_surface);

		if (!vk_context->create_logical_device(&queue_priorities, queue_count))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeWindow", "Create vulkan logical device faided");
			return false;
		}
		
		if(!vk_context->create_swap_chian(w, h))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "xeWindow", "Create vulkan swap chain faided");
			return false;
		}

		renderer_context = vk_context;
		window_context = window;

		return true;
	}

	const utf8_t* Window::get_window_title() const noexcept
	{
		auto window = std::any_cast<SDL_Window*>(window_context);
		return reinterpret_cast<const utf8_t*>(SDL_GetWindowTitle(window));
	}

	bool Window::get_window_size(int32_t& w, int32_t& h) const noexcept
	{
		auto window = std::any_cast<SDL_Window*>(window_context);
		return SDL_GetWindowSize(window, &w, &h);
	}

	void Window::show()
	{
		SDL_Event event = {};
		while (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_EventType::SDL_EVENT_QUIT)
			{
				break;
			}
		}
	}

	Window::~Window()
	{
		auto window = std::any_cast<SDL_Window*>(window_context);
		auto vk_context = std::any_cast<vulkan::VulkanContext*>(renderer_context);
		SDL_DestroyWindow(window);
		xe_delete(vk_context);
	}
} // namespace xe is end