#include "log/xeLogOutput.h"

#if defined(_DEBUG)
#define OPEN_CLR
#endif // is defined _DEBUG

#include <iostream>
#include <format>

// OUTPUT PATTERN
#if defined(_DEBUG)
#define OPEN_CLR
#endif // defined(_DEBUG)

namespace xe
{
	void XE_CLR_OUTPUT(const char* color, const char* color_string, const char* output_type, const char* addon_type, const char* output_text, const char* file, int line, const char* function)
	{
		std::string address = std::format("{0} line:{1} {2}(...)", file, line, function);
#if defined(OPEN_CLR)
		std::cout << std::string(color) + color_string + ":\t" + XE_CLR_COLOR::WHITE + "< " + output_type + " " + addon_type + " -> " + output_text + "> in " + address + "\n";
		return;
#endif // OPEN_CLR
		std::string out_text = std::string(color_string) + "< " + output_type + " " + addon_type + " -> " + output_text + "> in " + address + "\n";
		return;
	}
} // namespace xe is end