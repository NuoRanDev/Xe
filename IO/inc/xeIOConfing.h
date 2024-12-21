#ifndef _XE_IO_CONFIG_H_
#define _XE_IO_CONFIG_H_
#include "xeCore.hpp"

/************************************************
*                  Macro List
*  current porject
*  MACRO:	PROJECT_IS_XE_IO
*
*************************************************/

namespace xe
{
#ifdef PROJECT_IS_XE_IO
#define XE_IO_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_IO_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_IO IS END
}

#endif // _XE_IO_CONFIG_H_ IS END