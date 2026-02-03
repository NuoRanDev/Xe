#ifndef _INC_XE_DATA_STRUCTION_HPP_
#define _INC_XE_DATA_STRUCTION_HPP_

#include "type/xeOrdinals.hpp"

#include "memory/xeAlloc.hpp"

#include <vector>
#include <map>

namespace xe
{
	template<class struction1> using dynamic_array = std::vector<struction1, mi_stl_allocator<struction1>>;

	template<class struction1, class struction2> using map = std::map<struction1, struction2, mi_stl_allocator<std::pair<struction1, struction2>>>;
} // namespace xe is end

#endif // _INC_XE_DATA_STRUCTION_HPP_ IS EOF