import xe.Graphical.Widget;

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

#include "SDL3/SDL_vulkan.h"
#include "vulkan/vulkan.h"


import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.CoreClrOutput;
import xe.Core.xeBaseDataType;

namespace xe
{

	vec2_i32 xeWindow::GetWindowSize()
	{
		vec2_i32 window_size;
		SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(window), &window_size.x, &window_size.y);
		return window_size;
	}

	bool xeWindow::CreatWindow(xeInt32 w, xeInt32 h, xeString name, bool bordered)
	{
		SDL_WindowFlags window_flags;
#if defined(USE_OPENGL)
#define NOT_USE_VULKAN
#else
		window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS;
#endif // !NOT_USE_VULKAN

		if (!InitRenderApi())
		{
			XE_ERROR_OUTPUT(std::format("<CLASS: xeWindow> Failed to init reander API: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		window = reinterpret_cast<void*>(SDL_CreateWindow(name.c_str(), w, h, window_flags));
		if (!window)
		{
			XE_ERROR_OUTPUT(std::format("<LIB: SDL3> Failed to create window: {0}", SDL_GetError()).c_str());
			return false;
		}
		SDL_SetWindowPosition(reinterpret_cast<SDL_Window*>(window), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(reinterpret_cast<SDL_Window*>(window), bordered);

		if (!SDL_Vulkan_CreateSurface(reinterpret_cast<SDL_Window*>(window), window_vulkan_instance.GetInstance(), nullptr, window_vulkan_instance.GetSurface()))
		{
			XE_ERROR_OUTPUT(std::format("<LIB: SDL3> Failed to create vulkan surface: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		if(!window_vulkan_instance.SetVulkanLogicalDevice())
		{
			XE_ERROR_OUTPUT(std::format("<LIB : VULKAN> Failed to create vulkan logical device: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		return false;
	}

	bool xeWindow::InitRenderApi()
	{
		Uint32 count_instance_extensions;

		const char* const* sdl_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&count_instance_extensions);

		if (sdl_instance_extensions == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("<LIB: SDL3> Failed to get the number of required instance extensions: {0}\n", SDL_GetError()).c_str());
			return false;
		}

		if (!window_vulkan_instance.SetVulkanInstanceContext(sdl_instance_extensions, count_instance_extensions, "XE_SDL3_VK"))
		{
			XE_ERROR_OUTPUT("<LIB: SDL3> Failed to setup vulkan instance context");
			return false;
		}
		return true;
	}

	void xeWindow::StartWindowEvent()
	{
		window_envet_loop = std::thread(&xeWindow::WindowEnvetThread, this);
		XE_INFO_OUTPUT("<LIB: SDL3> Start window envet list");

		window_envet_loop.join();
		XE_INFO_OUTPUT("<LIB: SDL3> End window envet list");
	}

	void xeWindow::WindowRendering()
	{
		XE_INFO_OUTPUT("<LIB: SDL3> Start window render");
		SDL_Event event{};
		while (true)
		{
			if(SDL_WaitEvent(&event))
			{
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					goto ABORT_WINDOW_RENDER;

				case SDL_EVENT_KEY_DOWN:
					break;

				case SDL_EVENT_WINDOW_EXPOSED:
					break;
				}
			}
		}
	ABORT_WINDOW_RENDER:
		XE_INFO_OUTPUT("<LIB: SDL3> End window render");
	}

	xeWindow::~xeWindow()
	{
		SDL_DestroyWindow(reinterpret_cast<SDL_Window*>(window));
	}

	void xeWindow::WindowEnvetThread()
	{
	}
}