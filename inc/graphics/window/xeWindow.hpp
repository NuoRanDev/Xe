#ifndef _XE_WINDOW_HPP_
#define _XE_WINDOW_HPP_

#include "string/xeString.hpp"

#include "type/xeDataStruction.hpp"

#include <cstdint>
#include <queue>
#include <functional>
#include <any>
#include <mutex>

namespace xe
{
	constexpr int16_t MAX_WINDOW_WIDGET = SHRT_MAX;

	constexpr float DEFAULT_QUEUE_PRIORITIES = 1.0f;
	constexpr uint32_t DEFAULT_QUEUE_COUNT = 1;

	class Window
	{
	public:
		Window();

		bool create_window_context(const utf8_t* title, int32_t w, int32_t h, bool bordered) noexcept;

		const utf8_t* get_window_title() const noexcept;

		bool get_window_size(int32_t& w, int32_t& h) const noexcept;

		void show();

		~Window();

	private:

		uint32_t queue_count;

		float queue_priorities;

		std::any renderer_context;

		std::any window_context;

		const dynamic_array <const char*> want_extension_properties = { "VK_KHR_swapchain"};
	};
} // namespace xe is end

#endif // _XE_WINDOW_HPP_ IS EOF