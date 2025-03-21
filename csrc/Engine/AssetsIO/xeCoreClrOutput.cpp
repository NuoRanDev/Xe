#include <cstdio>
import xe.xeCore.xeCoreClrOutput;

import std;

namespace xe
{
	constexpr const char RESET[] = "\033[0m";
	constexpr const char RED[] = "\033[31m";
	constexpr const char GREEN[] = "\033[32m";
	constexpr const char YELLOW[] = "\033[33m";
	constexpr const char BLUE[] = "\033[34m";
	constexpr const char WHITE[] = "\033[37m";

	export void XE_ERROR_OUTPUT(const char* output_text)
	{
#ifdef _DEBUG
		std::cout << std::string(RED) + "ERROR" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // _DEBUG
		std::string out_text = std::string("ERROR") + ":\t" + output_text + "\n";
		fprintf(stderr, "Cannot read file: %s\n", out_text.c_str());
		return;
	}

	export void XE_WARNING_OUTPUT(const char* output_text)
	{
#ifdef _DEBUG
		std::cout << std::string(YELLOW) + "WARNING" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // _DEBUG
		std::string out_text = std::string("WARNING") + ":\t" + output_text + "\n";
		fprintf(stderr, "Cannot read file: %s\n", out_text.c_str());
		return;
	}
}