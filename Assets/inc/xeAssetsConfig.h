#ifndef _INC_XE_ASSETS_CONFIG_H_
#define _INC_XE_ASSETS_CONFIG_H_

#include "xeCore.hpp"
#include "xeIO.h"

/************************************************
*                  Macro List
*  current porject
*  MACRO:	PROJECT_IS_XE_ASSETS
*
*************************************************/

namespace xe
{
	constexpr uint64_t XE_DATA_COMPRESS_LZMA_PROPS_BYTE_SIZE = 4 * MB; // 4MB

#ifdef PROJECT_IS_XE_ASSETS
#define XE_ASSETS_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_ASSETS_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END
}

#endif // _INC_XE_ASSETS_CONFIG_H_ IS EOF