#ifndef __PRIVATE__FILE_TYPE_HPP_
#define __PRIVATE__FILE_TYPE_HPP_

#include "xeCoreConfig.hpp"
#include "xeBaseDataType.hpp"

namespace xe
{
	// PS : this has ##'.'##
	XE_EXPORT_C_PLUS_PLUS_API bool IsExtension(const char* main_csharp_str, int64_t len_1, const char* chird_csharp_str, int64_t len_2);
	// PS : this has ##'.'##
	XE_EXPORT_C_PLUS_PLUS_API bool IsExtension(const char* main_c_str, const char* chird_c_str);
	// 4 NAME
	XE_EXPORT_C_PLUS_PLUS_API bool Is4Extension(const char* main_csharp_str, int64_t len_1, xeFastExtension _4_extension);
}

#endif // __PRIVATE__FILE_TYPE_HPP_ IS EOF