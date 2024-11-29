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

	template<typename T> void DeleteArray(T* input)
	{
		if (input != nullptr)
		{
			delete[] input;
		}
		input = nullptr;
	}

	template<typename T> void DeleteSingla(T* input)
	{
		if (input != nullptr)
		{
			delete input;
		}
		input = nullptr;
	}

}
// Export function data
#if defined(_MSC_VER) // MSVC
#define EXPORT_HEAD __declspec(dllexport)
#elif defined(__GNUC__) //GCC
#define EXPORT_HEAD __attribute__((visibility("default")))
#endif // API EXPORT IS END

#if defined(USE_PROJECT_PRIVATE_API)
#define XE_CORE_PRIVATE_API				EXPORT_HEAD
#else
#define XE_CORE_PRIVATE_API
#endif // USE_PROJECT_PRIVATE_API IS END

#define EXTERN_C_STMT				extern"C"

#if defined(WIN32) || defined(_WIN32)
#define XE_CALL __stdcall
#else
#define XE_CALL __cdecl
#endif

#if defined(EXPORT_C_SHARP_API)

struct CsharpString
{
	int64_t _size;
	char* data;
};


#define XE_EXPORT_C_SHARP_API(ret_type) EXTERN_C_STMT EXPORT_HEAD ret_type XE_CALL
#define XE_EXPORT_C_PLUS_PLUS_API

#elif defined(EXPORT_C_PLUS_PLUS_API)

#define XE_EXPORT_C_SHARP_API
#define XE_EXPORT_C_PLUS_PLUS_API	EXPORT_HEAD

#endif

#endif // _INC_XE_CORE_CONFIG_H_ IS EOF