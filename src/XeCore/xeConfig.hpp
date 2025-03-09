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
	constexpr uint64_t KiB = 1024;
	constexpr uint64_t MiB = 1024 * KiB;
	constexpr uint64_t GiB = 1024 * MiB;
	constexpr uint64_t TiB = 1024 * GiB;
}

#ifdef PROJECT_IS_XE_CORE
#define XE_CORE_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_CORE_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END

#endif // _INC_XE_CORE_CONFIG_HPP_ IS EOF