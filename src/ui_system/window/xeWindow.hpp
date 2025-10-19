#ifndef _XE_WINDOW_HPP_
#define _XE_WINDOW_HPP_

#include "string/xeString.hpp"

#include <cstdint>
#include <queue>
#include <functional>
#include <any>
#include <mutex>

namespace xe
{
	constexpr int16_t MAX_WINDOW_WIDGET = SHRT_MAX;

	class Window
	{
	public:
		Window() 
		{
			command_map = nullptr;
			is_draw = new std::mutex();
		}

		bool create_window_context(int32_t w, int32_t h, xeString name, bool bordered) noexcept;

		bool draw_loop();

		void submit_draw_command(std::function<bool(Window*)> draw_cmd, bool is_stop_loop);

		int32_t get_width();

		int32_t get_height();

		~Window();

	private:
		
		friend class WindowManager;

		friend class Surface;

		friend class Texture;

#if defined(USE_OPENGL)
		void *context_opengl_instance;
#elif defined(USE_VULKAN)
		xeVulkanContext window_vulkan_instance;
#else
	public:
		
		void* get_render() { return std::any_cast<void*>(sdl_renderer); }

	private:
		
		std::any sdl_renderer;
#endif // !NOT_USE_VULKAN

		std::any sdl_window_contest;

		bool init_render_api();

		bool bind_reder_api_in_window();

		short *command_map;

		struct RenderCommand
		{
			std::function<bool(Window*)> draw_function;
			bool stop_loop;
		};

		// thread safe queue
		std::queue<RenderCommand> window_draw_functions;
		std::mutex *is_draw;

	};
} // namespace xe is end

#endif // _XE_WINDOW_HPP_ IS EOF