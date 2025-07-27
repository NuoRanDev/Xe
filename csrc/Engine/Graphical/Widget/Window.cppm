export module xe.Graphical.Widget;

import std;

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

		bool CreatWindow(xeInt32 w, xeInt32 h, xeString name, bool bordered = true);

		Vec2_i32 GetWindowSize();

		void StartWindowEvent();

		void WindowRendering();

		~xeWindow();

	private:

		bool InitRenderApi();

		void WindowEnvetThread();

		xeVulkanContext window_vulkan_instance;

		void* window;

		std::thread window_envet_loop;
	};
}