export module xe.Core.xeApplication;

import std;

namespace xe
{
	namespace Application
	{
		export bool LaodApplication(int argc, char** argv, const char* out_log_name);

		export const char8_t* GetApplicationPath();

		export const char8_t* GetApplicationName();

		export std::fstream* GetLogFile();

		export int DestroyApplication();
	}
}