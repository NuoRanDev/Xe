#include "xeCoreClrOutput.h"

#include "xeApplication.h"

#include <fstream>

// Clang warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

namespace xe 
{
	static std::ofstream g_log("program_output.log");

	constexpr std::string_view RESET = "\033[0m";
	constexpr std::string_view RED = "\033[31m";
	constexpr std::string_view GREEN = "\033[32m";
	constexpr std::string_view YELLOW = "\033[33m";
	constexpr std::string_view BLUE = "\033[34m";
	constexpr std::string_view WHITE = "\033[37m";

	void XE_ERROR_OUTPUT(const char* output_text)
	{
#ifdef _DEBUG
		std::cout << std::string(RED) + "ERROR" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // _DEBUG
		std::string out_text = std::string("ERROR") + ":\t" + output_text + "\n";
		Application::GetLogFile()->write(out_text.c_str(), out_text.size());
		return;
	}

	void XE_WARNING_OUTPUT(const char* output_text)
	{
#ifdef _DEBUG
		std::cout << std::string(YELLOW) + "WARNING" + ":\t" + std::string(WHITE) + output_text + "\n";
		return;
#endif // _DEBUG
		std::string out_text = std::string("WARNING") + ":\t" + output_text + "\n";
		Application::GetLogFile()->write(out_text.c_str(), out_text.size());
		return;
	}
}