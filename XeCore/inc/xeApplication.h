#ifndef _INC_XE_APPLICATION_H_
#define _INC_XE_APPLICATION_H_

#include "xeCoreConfig.h"

namespace xe
{
	namespace Application
	{
		void LaodApplication(int argc, char** argv, const char* out_log_name);

		std::string GetApplicationPath();

		std::string GetApplicationName();

		std::fstream* GetLogFile();

		int DestroyApplication();
	}
}

#endif // _INC_XE_APPLICATION_H_ IS EOF