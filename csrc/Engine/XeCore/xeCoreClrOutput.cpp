#include <cstdio>
import std;
import xe.Core.CoreClrOutput;

#define OPEN_CLR

namespace xe
{
	constexpr const char RESET[] = "\033[0m";
	constexpr const char RED[] = "\033[31m";
	constexpr const char GREEN[] = "\033[32m";
	constexpr const char YELLOW[] = "\033[33m";
	constexpr const char BLUE[] = "\033[34m";
	constexpr const char WHITE[] = "\033[37m";

	void XE_ERROR_OUTPUT(const char* output_text)
	{
#if defined(OPEN_CLR)
		std::cout << std::string(RED) + "ERROR" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // OPEN_CLR
		std::string out_text = std::string("ERROR") + ":\t" + output_text + "\n";
		fprintf(stderr, "Cannot read file: %s\n", out_text.c_str());
		return;
	}

	void XE_WARNING_OUTPUT(const char* output_text)
	{
#if defined(OPEN_CLR)
		std::cout << std::string(YELLOW) + "WARNING" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // OPEN_CLR
		std::string out_text = std::string("WARNING") + ":\t" + output_text + "\n";
		fprintf(stderr, "Cannot read file: %s\n", out_text.c_str());
		return;
	}

	void XE_INFO_OUTPUT(const char* output_text)
	{
#if defined(OPEN_CLR)
		std::cout << std::string(BLUE) + "LOG" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // OPEN_CLR
		std::string out_text = std::string("LOG") + ":\t" + output_text + "\n";
		fprintf(stderr, "Cannot read file: %s\n", out_text.c_str());
		return;
	}
}