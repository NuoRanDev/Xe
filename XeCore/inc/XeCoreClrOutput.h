#ifndef _INC_XE_COER_CLR_OUTPUT_H_
#define _INC_XE_COER_CLR_OUTPUT_H_

#include "xeCoreConfig.h"

constexpr std::string_view RESET	= "\033[0m";
constexpr std::string_view RED		= "\033[31m";
constexpr std::string_view GREEN	= "\033[32m";
constexpr std::string_view YELLOW	= "\033[33m";
constexpr std::string_view BLUE		= "\033[34m";
constexpr std::string_view WHITE	= "\033[37m";

EXTERN_C_STMT void XE_CORE_PRIVATE_API XE_ERROR_OUTPUT(const char* output_text);
EXTERN_C_STMT void XE_CORE_PRIVATE_API XE_WARNING_OUTPUT(const char* output_text);

#endif // _INC_XE_COER_CLR_OUTPUT_H_ IS EOF