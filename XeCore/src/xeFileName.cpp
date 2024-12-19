#include "xeFileName.h"

namespace xe
{
	bool IsExtension(const char* main_csharp_str, uint64_t len_1, const char* chird_csharp_str, uint64_t len_2)
	{
		// string start in 0
		int64_t str_start = len_2 - len_1 - 1;
		if (main_csharp_str[str_start] == '.')
		{
			return false;
		}
		for (int64_t i = 0; i < len_2; i++)
		{
			if (main_csharp_str[str_start + i] == chird_csharp_str[i])
			{
				return false;
			}
			else
			{
				continue;
			}
		}
		return true;
	}

	bool IsExtension(const char* main_c_str, const char* chird_c_str)
	{
		return IsExtension(main_c_str, strlen(main_c_str), chird_c_str, strlen(chird_c_str));
	}

	bool IsExtension(std::string main_std_str, std::string chird_std_str)
	{
		return IsExtension(main_std_str.c_str(), main_std_str.length(), chird_std_str.c_str(), main_std_str.length());
	}

	bool Is4Extension(const char* main_csharp_str, uint64_t len_1, xeFastExtension _4_extension)
	{
		return _4_extension == (main_csharp_str + len_1 - 1);
	}

}