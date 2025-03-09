#ifndef _INC_XE_TESTURE_CONFIG_HPP_
#define _INC_XE_TESTURE_CONFIG_HPP_

/********************************************
* current porject
* MACRO:	PROJECT_IS_XE_TESTURE
*
*************************************************/

#include "xeConfig.hpp"

namespace xe
{
#ifdef PROJECT_IS_XE_TESTURE
#define XE_TESTURE_EXPORT_C_PLUS_PLUS_API XE_EXPORT_C_PLUS_PLUS_API
#else
#define XE_TESTURE_EXPORT_C_PLUS_PLUS_API
#endif // PROJECT_IS_XE_ASSETS IS END
}

#endif // _INC_XE_TESTURE_CONFIG_HPP_ IS EOF