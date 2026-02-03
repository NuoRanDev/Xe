#ifndef _XE_WINDOW_HPP_
#define _XE_WINDOW_HPP_

#include "string/xeString.hpp"

#include "graph/graph.hpp"

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
		Window();

		bool create_window_context(const char* exe_name, int32_t w, int32_t h, String name, bool bordered) noexcept;

		bool draw_loop();

		void submit_draw_command(std::function<bool(Window*)> draw_cmd, bool is_stop_loop);

		int32_t get_width();

		int32_t get_height();

		~Window();

	private:
		
		friend class WindowManager;

		friend class Surface;

		friend class Texture;

		friend class FragmentShader;

	public:
		
		void* get_render() { return std::any_cast<void*>(renderer_context); }

	private:
		
		std::any renderer_context;

		std::any sdl_window_context;

		bool init_render_api(const char* exe_name) noexcept;

		bool bind_render_api_in_window();

		GraphBit16 command_map;

		struct RenderCommand
		{
			std::function<bool(Window*)> draw_function;
			bool stop_loop;
		};

		// thread safe queue
		std::queue<RenderCommand> window_draw_functions[2];
		uint32_t current_draw_function_index = 0;
		std::mutex *is_draw;

	};
} // namespace xe is end

#endif // _XE_WINDOW_HPP_ IS EOF