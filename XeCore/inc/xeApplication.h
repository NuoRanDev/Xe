#ifndef _INC_XE_APPLICATION_H_
#define _INC_XE_APPLICATION_H_

#include "xeCoreConfig.h"

namespace xe
{
	namespace Application
	{
		EXTERN_C_STMT XE_EXPORT_C_PLUS_PLUS_API void LaodApplication(int argc, char** argv, const char* out_log_name);

		EXTERN_C_STMT XE_EXPORT_C_PLUS_PLUS_API const char* GetApplicationPath();

		EXTERN_C_STMT XE_EXPORT_C_PLUS_PLUS_API const char* GetApplicationName();

		EXTERN_C_STMT XE_EXPORT_C_PLUS_PLUS_API std::fstream* GetLogFile();

		EXTERN_C_STMT XE_EXPORT_C_PLUS_PLUS_API int DestroyApplication();
	}
}

#endif // _INC_XE_APPLICATION_H_ IS EOF