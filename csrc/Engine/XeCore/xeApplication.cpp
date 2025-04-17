import xe.Core.xeApplication;

#include "SDL3/SDL_init.h"
import std;

import xe.Core.CoreClrOutput;

namespace xe
{
	namespace Application
	{
		static std::u8string application_path;
		static std::u8string application_name;

		bool LaodApplication(int argc, char** argv)
		{
			std::filesystem::path file_name_with_path = argv[0];
			application_path = file_name_with_path.remove_filename().u8string();
			application_name = file_name_with_path.filename().u8string();
#if defined(_WIN32)
			std::system("chcp 65001");
			std::system("cls");
#endif // _WIN32

			if (!SDL_Init(SDL_INIT_VIDEO))
			{
				XE_ERROR_OUTPUT(std::format("<LIB: SDL3> Failed to initialize SDL: {0}", SDL_GetError()).c_str());
				return false;
			}
			return true;
		}

		const char8_t* GetApplicationPath()
		{
			return application_path.c_str();
		}

		const char8_t* GetApplicationName()
		{
			return application_name.c_str();
		}

		int DestroyApplication()
		{
			SDL_Quit();
			return 0;
		}
	}
}