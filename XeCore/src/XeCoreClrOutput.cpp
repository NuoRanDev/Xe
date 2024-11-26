#include "XeCoreClrOutput.h"

void XE_ERROR_OUTPUT(const char* output_text)
{
#ifdef _DEBUG
	std::cout << std::string(RED) + "ERROR" + ":\t" + std::string(WHITE) + output_text << std::endl;
#endif // DEBUG IS END
}

void XE_WARNING_OUTPUT(const char* output_text)
{
#ifdef _DEBUG
	std::cout << std::string(RED) + "WARNING" + ":\t" + std::string(WHITE) + output_text << std::endl;
#endif // DEBUG IS END
}