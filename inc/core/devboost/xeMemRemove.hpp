#ifndef _XE_MEM_REMOVE_HPP_
#define _XE_MEM_REMOVE_HPP_

#include "type/xeOrdinals.hpp"

namespace xe
{
	void* memerase(byte_t* src, uint64_t src_size, uint64_t start, uint64_t ofs) noexcept;
} // namespace xe is end

#endif // !_XE_MEM_REMOVE_HPP_ IS EOF