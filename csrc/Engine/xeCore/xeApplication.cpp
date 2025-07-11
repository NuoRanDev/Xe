import xe.Core.xeApplication;

#include "SDL3/SDL_init.h"
import std;

import xe.Core.xeAlloc;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;

#if !defined(EXPORT_C_SHARP_API)
import xe.Core.xeString;
#endif //!defined(EXPORT_C_SHARP_API)

namespace xe
{
	namespace Application
	{
		bool InitGui()
		{
			XE_DEBUG_OUTPUT("Init SDL");
			if (!SDL_Init(SDL_INIT_VIDEO))
			{
				XE_ERROR_OUTPUT(std::format("<LIB: SDL3> Failed to initialize SDL: {0}", SDL_GetError()).c_str());
				return false;
			}
			return true;
		}

#if !defined(EXPORT_C_SHARP_API)
		static xeString application_path;
		static xeString application_name;

		bool LaodApplication(int argc, char** argv)
		{
			std::filesystem::path file_name_with_path = argv[0];

			auto u8_application_path = file_name_with_path.remove_filename().u8string();

			application_path = xeString(u8_application_path.data(), static_cast<xeInt64>(u8_application_path.size()));

			auto u8_application_name = file_name_with_path.filename().u8string();

			application_name = xeString(u8_application_name.data(), static_cast<xeInt64>(u8_application_name.size()));
#else
		bool LaodApplication()
		{
#endif // !defined(EXPORT_C_SHARP_API)
			bool is_success = true;
#if defined(_WIN32)
			std::system("chcp 65001");
			std::system("cls");
#endif // _WIN32
			is_success = InitGui();
			if (!is_success)
			{
				return is_success;
			}
			return is_success;
		}

#if !defined(EXPORT_C_SHARP_API)
		xeString GetApplicationPath()
		{
			return application_path;
		}

		xeString GetApplicationName()
		{
			return application_name;
		}
#endif // !defined(EXPORT_C_SHARP_API)

		void DestroyGui()
		{
			SDL_Quit();
		}

		int DestroyApplication()
		{
			DestroyGui();
			return 0;
		}
	}
}
