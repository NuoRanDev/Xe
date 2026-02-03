#ifndef _XE_ORDINALA_HPP_
#define _XE_ORDINALA_HPP_

#include <cstdint>
#include <type_traits>

namespace xe
{
	using unicode_t = char32_t;
	using byte_t = uint8_t;

	using utf8_t = char8_t;

	using any_type_ptr_t = void*;

	template<typename T>
	concept IntegralType = requires(T)
	{
		std::is_integral<T>::value;
	};
} // namespace xe is end

#endif // _XE_ORDINALA_HPP_ IS END