#ifndef _INC_XE_CORE_CONFIG_H_
#define _INC_XE_CORE_CONFIG_H_

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
*  BUILD THIS PROJECT MUST DEFINEED 
*  MACRO:	USE_PROJECT_PRIVATE_API
* 
*  current porject
*  MACRO:	PROJECT_IS_XE_CORE
* 
*************************************************/

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
// Export function data
#if defined(_MSC_VER) // MSVC
#define EXPORT_HEAD __declspec(dllexport)
#elif defined(__GNUC__) //GCC
#define EXPORT_HEAD __attribute__((visibility("default")))
#endif // API EXPORT IS END

/*
#if defined(USE_PROJECT_PRIVATE_API)
#define XE_PRIVATE_FUNCTION		EXPORT_HEAD
#else
#define XE_PRIVATE_FUNCTION
#endif // USE_PROJECT_PRIVATE_API IS END
*/

#define EXTERN_C_STMT				extern"C"

#ifndef XE_CALL
#if defined(WIN32) || defined(_WIN32)
#define XE_CALL __stdcall
#else
#define XE_CALL __cdecl
#endif
#endif


#if defined(EXPORT_C_SHARP_API)

#define XE_EXPORT_C_SHARP_API(ret_type) EXTERN_C_STMT EXPORT_HEAD ret_type XE_CALL
#define XE_EXPORT_C_PLUS_PLUS_API

#elif defined(EXPORT_C_PLUS_PLUS_API)

#define XE_EXPORT_C_SHARP_API
#define XE_EXPORT_C_PLUS_PLUS_API	EXPORT_HEAD

#endif
/*
#if defined(_WIN32)
// Windows use 16-bits string (utf-16)
using SystemDefaultString = wchar_t;
#else
// Other Systems use 8-bits string (utf-8)
using SystemDefaultString = char;
#endif //String Support
*/
#ifdef PROJECT_IS_XE_CORE
#define XE_CORE_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_CORE_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END

#endif // _INC_XE_CORE_CONFIG_H_ IS EOF