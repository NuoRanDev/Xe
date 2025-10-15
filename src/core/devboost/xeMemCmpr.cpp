#include "devboost/xeMemCmp.h"

#include <mmintrin.h>
#include <immintrin.h>

namespace xe
{
	bool short_memory_cmp(const byte_t* cmp1, const byte_t* cmp2, const uint64_t cmp_size) noexcept
	{
		for (uint64_t i = 0; i < cmp_size; i++)
		{
			if (cmp1[i] != cmp2[i]) return false;
		}
		return true;
	}

	bool long_memory_cmp(const byte_t* cmp1, const byte_t* cmp2, const uint64_t cmp_size) noexcept
	{
		uint64_t i = 0;
		__m256i cmp1_16_byte, cmp2_16_byte, cmp_result;

		while (i < cmp_size)
		{
			cmp1_16_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cmp1));
			cmp2_16_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cmp2));

			cmp_result = _mm256_cmpeq_epi8(cmp1_16_byte, cmp2_16_byte);
			// if all 32 byte are equal, then cmp_result will be 0xFF
			// if not equal, then cmp_result will be 0x00
			auto cmp_result_mask = _mm256_movemask_epi8(cmp_result);
			if (cmp_result_mask != 0b1111'1111'1111'1111'1111'1111'1111'1111)
			{
				// if not all 32 byte are equal, then return false
				return false;
			}
			cmp1 += 32;
			cmp2 += 32;
			i += 32;
		}
		if (i == cmp_size)
		{
			return true; // all bytes are equal
		}
		// if cur_cmp_size < 32, then use short memory compare
		int mask = 0 << (i - cmp_size);
		cmp1_16_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cmp1));
		cmp2_16_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cmp2));

		cmp_result = _mm256_cmpeq_epi8(cmp1_16_byte, cmp2_16_byte);

		auto cmp_result_mask = _mm256_movemask_epi8(cmp_result);
		if (cmp_result_mask != mask)
		{
			return false;
		}
		return true; // all bytes are equal
	}

	std::vector<int64_t> find_byte_all_memory_int64size(const byte_t* str, const uint64_t size, const byte_t c) noexcept
	{
		std::vector<int64_t> result;
		if (size == 0 || str == nullptr)
			return result;
		for (int64_t i = 0; i < static_cast<int64_t>(size); i++)
		{
			if (str[i] == c)
			{
				result.push_back(i);
			}
		}
		return result;
	}
} // namespace xe is end