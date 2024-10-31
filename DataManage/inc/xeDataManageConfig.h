#ifndef _INC_XE_DATA_MANAGE_H_
#define _INC_XE_DATA_MANAGE_H_

#include <cmath>
#include <cstdio>
#include <format>
#include <mutex>
#include <thread>
#include <fstream>
#include <iostream>
#include <format>

#include "xeCore.h"

namespace xe
{
	constexpr uint64_t XE_DATA_COMPRESS_LZMA_PROPS_BYTE_SIZE = 4 * MB; // 4MB
}

#endif // _INC_XE_DATA_MANAGE_H_ IS EOF