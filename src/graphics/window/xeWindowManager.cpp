#include "window/xeWindowManager.hpp"

#include "SDL3/SDL_init.h"

#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

#include <format>

namespace xe
{
	WindowManager::WindowManager(char** argv) noexcept
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeWindowManager : SDL3", std::format("Init Error: {0}", SDL_GetError()).c_str());
		}
		exe_name = argv[0];
		displays = SDL_GetDisplays(&displays_count);
	}

	Window* WindowManager::create_window(int32_t w, int32_t h, String& title, bool bordered) noexcept
	{
		Window* new_window = new Window();

		if (!new_window->create_window_context(title.data(), w, h, bordered))
		{
			xe_delete(new_window);
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "xeWindowManager : SDL3", std::format("SDL Init Error: {0}", SDL_GetError()).c_str());
			return nullptr;
		}
		window_list.push_back(new_window);
		return new_window;
	}

	Window* WindowManager::get_window(String& name) noexcept
	{
		for (auto& window : window_list)
		{
			if (name == window->get_window_title())
				return window;
		}
		return nullptr;
	}

	WindowManager::~WindowManager()
	{
		for (auto& window : window_list)
		{
			xe_delete(window);
		}
		SDL_free(displays);
		SDL_Quit();
	}
} // namespace xe is end