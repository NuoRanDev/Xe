#ifndef _INC_XE_MEM_FIND_HPP_
#define _INC_XE_MEM_FIND_HPP_

#include "type/xeOrdinals.hpp"

namespace xe
{
	int64_t memfind(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area);

	//int64_t memfindlast(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area);
} // namespace xe is end

#endif // _INC_XE_MEM_FIND_HPP_ IS EOF