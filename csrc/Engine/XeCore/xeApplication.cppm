export module xe.Core.xeApplication;

import std;

import xe.Core.xeOrdinals;

#include "xeCompiler.hpp"

namespace xe
{
	namespace Application 
	{
#if !defined(EXPORT_C_SHARP_API)
		export bool LaodApplication(int argc, char** argv);

		export const xe::xeU8cstr* GetApplicationPath();

		export const xe::xeU8cstr* GetApplicationName();
#else
		export bool LaodApplication();
#endif // !defined(EXPORT_C_SHARP_API)

		export int DestroyApplication();
	}
}