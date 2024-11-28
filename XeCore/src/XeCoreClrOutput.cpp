#include "XeCoreClrOutput.h"

#include <fstream>

namespace xe 
{
	static std::ofstream g_log("program_output.log");

	constexpr std::string_view RESET = "\033[0m";
	constexpr std::string_view RED = "\033[31m";
	constexpr std::string_view GREEN = "\033[32m";
	constexpr std::string_view YELLOW = "\033[33m";
	constexpr std::string_view BLUE = "\033[34m";
	constexpr std::string_view WHITE = "\033[37m";

	void SetLogOutput()
	{
#ifdef _DEBUG
		return;
#endif // _DEBUG
		std::cout.rdbuf(g_log.rdbuf());
		return;
	}

	void XE_ERROR_OUTPUT(const char* output_text)
	{
		std::cout << std::string(RED) + "ERROR" + ":\t" + std::string(WHITE) + output_text << std::endl;
	}

	void XE_WARNING_OUTPUT(const char* output_text)
	{
		std::cout << std::string(RED) + "WARNING" + ":\t" + std::string(WHITE) + output_text << std::endl;
	}
}