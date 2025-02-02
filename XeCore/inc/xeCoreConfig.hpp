#ifndef _INC_XE_CORE_CONFIG_HPP_
#define _INC_XE_CORE_CONFIG_HPP_

#include <format>
#include <iostream>
#include <locale>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <functional>
#include <string_view>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
/************************************************
*                  Macro List
*  want to export cpp api
*  MACRO:	EXPORT_C_PLUS_PLUS_API
* 
*  want to export C api to c-sharp link  
*  MACRO:	EXPORT_C_SHARP_API
* 
*  use mimalloc alloc memory
*  MACRO:	USE_MIMALLOC
* 
*  current porject
*  MACRO:	PROJECT_IS_XE_CORE
* 
*************************************************/

// compiler marco
#include "xeCompiler.hpp"

namespace xe
{
	using offsetptr_t = void*;
	using byte_t = uint8_t;

	constexpr uint64_t KB = 1024;
	constexpr uint64_t MB = 1024 * 1024;
	constexpr uint64_t GB = 1024 * 1024 * 1024;

	template<typename T> uint32_t ARRAYSIZE(T* _ARR)
	{
		return ((int)(sizeof(_ARR) / sizeof(*(_ARR))));
	}

	template<typename T> T* DeleteArray(T* input)
	{
		if (input != nullptr)
		{
			delete[] input;
		}
		input = nullptr;
		return nullptr;
	}

	template<typename T> T* DeleteSingla(T* input)
	{
		if (input != nullptr)
		{
			delete input;
		}
		input = nullptr;
		return nullptr;
	}

}

#ifdef PROJECT_IS_XE_CORE
#define XE_CORE_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_CORE_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END

#endif // _INC_XE_CORE_CONFIG_HPP_ IS EOF