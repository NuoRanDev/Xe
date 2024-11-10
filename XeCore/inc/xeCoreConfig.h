#ifndef _INC_XE_CORE_CONFIG_H_
#define _INC_XE_CORE_CONFIG_H_

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <locale>
#include <string>

/************************************************
*                  Macro List
*  EXPORT_C_PLUS_PLUS_API export cpp api
*  EXPORT_C_SHARP_API has exported C api to c-sharp link  
* 
* 
* 
*************************************************/

namespace xe
{
	using offsetptr_t = void*;
	using byte_t = uint8_t;

	constexpr uint64_t KB = 1024;
	constexpr uint64_t MB = 1024 * 1024;
	constexpr uint64_t GB = 1024 * 1024 * 1024;
}

#if defined(_MSC_VER) // MSVC
#define EXPORT_HEAD __declspec(dllexport)
#elif defined(__GNUC__) //GCC
#define EXPORT_HEAD __attribute__((visibility("default")))
#endif // API EXPORT IS END

#define XE_PRIVATE_API				EXPORT_HEAD

#if defined(EXPORT_C_SHARP_API)

struct CsharpString
{
	int64_t _size;
	char* data;
};

#define XE_EXPORT_C_SHARP_API		EXPORT_HEAD extern"C" __stdcall
#define XE_EXPORT_C_PLUS_PLUS_API

#elif defined(EXPORT_C_PLUS_PLUS_API)

#define XE_EXPORT_C_SHARP_API
#define XE_EXPORT_C_PLUS_PLUS_API	EXPORT_HEAD

#endif

#endif // _INC_XE_CORE_CONFIG_H_ IS EOF