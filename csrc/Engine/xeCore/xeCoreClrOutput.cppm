export module xe.Core.CoreClrOutput;

import std;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

namespace xe
{
	export void XE_ERROR_OUTPUT(const char* output_text);

	export void XE_WARNING_OUTPUT(const char* output_text);

	export void XE_INFO_OUTPUT(const char* output_text);

	export void XE_DEBUG_OUTPUT(const char* output_text);
}