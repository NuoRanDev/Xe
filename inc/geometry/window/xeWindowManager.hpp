#ifndef _XE_WINDOW_MANAGER_HPP_
#define _XE_WINDOW_MANAGER_HPP_

#include "window/xeWindow.hpp"

#include <map>

namespace xe
{
	class Window;
	class WindowManager
	{
	public:
		WindowManager();

		bool create_window(int32_t w, int32_t h, xeString name, bool bordered) noexcept;

		Window* get_window(xeString name) noexcept;

		~WindowManager();

	private:

		std::map<xeString, Window*> window_map;

	};
} // namespace xe is end

#endif // _XE_WINDOW_MANAGER_HPP_