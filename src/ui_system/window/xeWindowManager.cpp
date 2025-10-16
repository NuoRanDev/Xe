#include "window/xeWindowManager.hpp"

#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.hpp"

#include <format>

namespace xe
{
	WindowManager::WindowManager()
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", std::format("Init Error: {0}", SDL_GetError()).c_str());
		}
	}

	bool WindowManager::create_window(int32_t w, int32_t h, xeString name, bool bordered) noexcept
	{
		Window* new_window = new Window();
		if (!new_window->create_window_context(w, h, name, bordered))
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeUISystem : SDL3", std::format("SDL Init Error: {0}", SDL_GetError()).c_str());
			xe_delete(new_window);
		}
		window_map[name] = new_window;
		return true;
	}

	Window* WindowManager::get_window(xeString name) noexcept
	{
		return window_map[name];
	}

	WindowManager::~WindowManager()
	{
		for (auto& window : window_map)
		{
			xe_delete(window.second);
		}
		SDL_Quit();
	}
} // namespace xe is end