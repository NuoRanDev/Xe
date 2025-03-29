export module xe.Graphical.Widget;

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

		void StartWindowEnvet();

		void WindowRendering();

		~xeWindow();

	private:

		void WindowEnvetThtead();

		xeVulkanContext window_vulkan_instance;

		SDL_Window* window;

		std::thread window_envet_loop;
	};
}