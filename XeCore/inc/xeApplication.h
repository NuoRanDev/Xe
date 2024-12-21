#ifndef _INC_XE_APPLICATION_H_
#define _INC_XE_APPLICATION_H_

#include "xeCoreConfig.h"

namespace xe
{
	namespace Application
	{
		XE_CORE_EXPORT_C_PLUS_PLUS_API void LaodApplication(int argc, char** argv, const char* out_log_name);

		XE_CORE_EXPORT_C_PLUS_PLUS_API std::string GetApplicationPath();

		XE_CORE_EXPORT_C_PLUS_PLUS_API std::string GetApplicationName();

		XE_CORE_EXPORT_C_PLUS_PLUS_API std::fstream* GetLogFile();

		XE_CORE_EXPORT_C_PLUS_PLUS_API int DestroyApplication();
	}
}

#endif // _INC_XE_APPLICATION_H_ IS EOF