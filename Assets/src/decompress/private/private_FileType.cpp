#include "decompress/private/private_FileType.h"

namespace xe
{
	bool GetNameExtension(const char* main_csharp_str, int64_t len_1, const char* chird_csharp_str, int64_t len_2)
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

	bool GetNameExtension(const char* main_c_str, const char* chird_c_str)
	{
		return GetNameExtension(main_c_str, strlen(main_c_str), chird_c_str, strlen(chird_c_str));
	}

	bool GetNameExtension(std::string main_std_str, std::string chird_std_str)
	{
		return GetNameExtension(main_std_str.c_str(), chird_std_str.c_str());
	}

}