import xe.Graphical.Widget.Window;

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"
#include "vulkan/vulkan.h"

import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.CoreClrOutput;

namespace xe
{
	bool xeWindow::CreatWindow(xeInt32 w, xeInt32 h, xeString name)
	{
		if (!SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			XE_ERROR_OUTPUT(std::format("LIB <SDL> :Failed to initialize SDL: {0}", SDL_GetError()).c_str());
			return false;
		}
		window = SDL_CreateWindow(name.c_str(), w, h, SDL_WINDOW_VULKAN);
		if (!window) 
		{
			XE_ERROR_OUTPUT(std::format("Failed to create window: {0}", SDL_GetError()).c_str());
			return false;
		}
		return true;
	}

	vec2_i32 xeWindow::GetWindowSize()
	{
		vec2_i32 window_size;
		SDL_GetWindowSize(window, &window_size.x, &window_size.y);
		return window_size;
	}

	bool xeWindow::InitWindowVulkan()
	{
		Uint32 count_instance_extensions;
		const char* const* sdl_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&count_instance_extensions);

		if(window_vulkan_instance.SetupVulkanContext(sdl_instance_extensions, count_instance_extensions, "XE_SDL3_VK"))
		{
			XE_ERROR_OUTPUT("<LIB : SDL3> Failed to setup Vulkan context");
			return false;
		}
		if (sdl_instance_extensions == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("<LIB : SDL3> Failed to get the number of required instance extensions: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		if (SDL_Vulkan_CreateSurface(window, window_vulkan_instance.GetInstance(), nullptr, window_vulkan_instance.GetSurface()) == false)
		{
			XE_ERROR_OUTPUT(std::format("Failed to create Vulkan surface: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		VkBool32 presentSupport = false;
		//vkGetPhysicalDeviceSurfaceSupportKHR(window_vulkan_instance.GetPhysicalDevice(), i, *(window_vulkan_instance.GetSurface()), &presentSupport);
		return false;
	}

	xeWindow::~xeWindow()
	{
		window_vulkan_instance.CleanupVulkanContext();
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}