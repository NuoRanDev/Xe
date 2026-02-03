#ifndef _XE_WINDOW_MANAGER_HPP_
#define _XE_WINDOW_MANAGER_HPP_

#include "string/xeString.hpp"

namespace xe
{
	class Window;
	class WindowManager
	{
	public:
		WindowManager(const char* i_exe_name);

		bool create_window(int32_t w, int32_t h, String name, bool bordered) noexcept;

		Window* get_window(String name) noexcept;

		~WindowManager();

	private:

		dynamic_array<Window*> window_list;

		const char* exe_name;

	};
} // namespace xe is end

#endif // _XE_WINDOW_MANAGER_HPP_