#ifndef _XE_WINDOW_MANAGER_HPP_
#define _XE_WINDOW_MANAGER_HPP_

#include "string/xeString.hpp"
#include "window/xeWindow.hpp"

namespace xe
{
	class WindowManager
	{
	public:
		WindowManager(const char* i_exe_name);

		Window* create_window(int32_t w, int32_t h, String& title, bool bordered = true) noexcept;

		Window* get_window(String& name) noexcept;

		~WindowManager();

	private:

		int32_t displays_count;

		dynamic_array<Window*> window_list;

		const char* exe_name;

	};
} // namespace xe is end

#endif // _XE_WINDOW_MANAGER_HPP_