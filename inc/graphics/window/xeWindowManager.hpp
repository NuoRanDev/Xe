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
		WindowManager(const char* i_exe_name);

		bool create_window(int32_t w, int32_t h, xeString name, bool bordered) noexcept;

		Window* get_window(xeString name) noexcept;

		~WindowManager();

	private:

		std::map<xeString, Window*> window_map;

		const char* exe_name;

	};
} // namespace xe is end

#endif // _XE_WINDOW_MANAGER_HPP_