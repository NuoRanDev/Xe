#ifndef __PRIVATE__FILE_TYPE_H_
#define __PRIVATE__FILE_TYPE_H_

#include "xeAssetsConfig.h"

namespace xe
{
	// PS : this has ##'.'##
	bool GetNameExtension(const char* main_csharp_str, int64_t len_1, const char* chird_csharp_str, int64_t len_2);
	// PS : this has ##'.'##
	bool GetNameExtension(const char* main_c_str, const char* chird_c_str);
	// PS : this has ##'.'##
	bool GetNameExtension(std::string main_std_str, std::string chird_std_str);
}

#endif // __PRIVATE__FILE_TYPE_H_ IS EOF