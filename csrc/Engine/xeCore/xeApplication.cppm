export module xe.Core.xeApplication;

import std;

import xe.Core.xeOrdinals;
#if !defined(EXPORT_C_SHARP_API)
import xe.Core.xeString;
#else
#include "xeCompiler.hpp"
#endif // !defined(EXPORT_C_SHARP_API)

namespace xe
{
	namespace Application 
	{
#if !defined(EXPORT_C_SHARP_API)

		export xeString GetApplicationPath();

		export xeString GetApplicationName();

		export bool LaodApplication(int argc, char** argv);
#else
		export bool LaodApplication();
#endif // !defined(EXPORT_C_SHARP_API)

		export int DestroyApplication();
	}
}