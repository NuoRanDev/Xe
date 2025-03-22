export module xe.Graphical.Widget.Window;

import std;

import "SDL3/SDL.h";

import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.xeBaseDataType;

import xe.Graphical.Vulkan;

namespace xe
{
	export class xeWindow
	{
	public:
		xeWindow() = default;

		bool CreatWindow(xeInt32 w, xeInt32 h, xeString name);

		vec2_i32 GetWindowSize();

		bool InitWindowVulkan();

		~xeWindow();

	private:
		xeVulkanContext window_vulkan_instance;

		SDL_Window* window;
	};
}