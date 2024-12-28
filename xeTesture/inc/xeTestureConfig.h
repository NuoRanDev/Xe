#ifndef _INC_XE_TESTURE_CONFIG_H_
#define _INC_XE_TESTURE_CONFIG_H_

/********************************************
* current porject
* MACRO:	PROJECT_IS_XE_TESTURE
*
*************************************************/

#include "xeCore.hpp"

namespace xe
{
#ifdef PROJECT_IS_XE_TESTURE
#define XE_TESTURE_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_TESTURE_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END
}

#endif // _INC_XE_TESTURE_CONFIG_H_ IS EOF