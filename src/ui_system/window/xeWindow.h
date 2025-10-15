#ifndef _XE_WINDOW_H_
#define _XE_WINDOW_H_

#include "string/xeString.h"

#include <cstdint>

namespace xe
{
	class Window
	{
	public:
		Window() = default;

		bool create_window_context(int32_t w, int32_t h, xeString name, bool bordered) noexcept;

		bool main_loop();

		~Window();

	private:
#if defined(USE_OPENGL)
		void *context_opengl_instance;
#elif defined(USE_VULKAN)
		xeVulkanContext window_vulkan_instance;
#else
		void* sdl_renderer;
#endif // !NOT_USE_VULKAN

		void* sdl_window_contest;

		bool init_render_api();

		bool bind_reder_api_in_window();

	};
} // namespace xe is end

#endif // _XE_WINDOW_H_ IS EOF