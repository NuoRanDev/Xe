#ifndef __PRIVATE__FILE_TYPE_H_
#define __PRIVATE__FILE_TYPE_H_

#include "xeCoreConfig.h"
#include "xeBaseDataType.h"

namespace xe
{
	// PS : this has ##'.'##
	bool IsExtension(const char* main_csharp_str, uint64_t len_1, const char* chird_csharp_str, uint64_t len_2);
	// PS : this has ##'.'##
	bool IsExtension(const char* main_c_str, const char* chird_c_str);
	// PS : this has ##'.'##
	bool IsExtension(std::string main_std_str, std::string chird_std_str);
	// 4 NAME
	bool Is4Extension(const char* main_csharp_str, uint64_t len_1, xeFastExtension _4_extension);
}

#endif // __PRIVATE__FILE_TYPE_H_ IS EOF