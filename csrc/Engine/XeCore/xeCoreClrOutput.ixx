export module xe.xeCore.xeCoreClrOutput;

import std;

import xe.xeCore.xeApplication;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

#include <cstdio>
namespace xe
{
	export void XE_ERROR_OUTPUT(const char* output_text);

	export void XE_WARNING_OUTPUT(const char* output_text);
}