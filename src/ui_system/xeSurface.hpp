#ifndef _XE_SURFACE_HPP_
#define _XE_SURFACE_HPP_

#include <cstdint>

#include "string/xeString.hpp"

namespace xe
{
	class Surface
	{
	public:

		Surface() = default;
		
		~Surface() = default;

		void on_update();
		void on_render();

		int32_t get_x() const noexcept { return x; }

		int32_t get_y() const noexcept { return y; }
		
		int32_t get_width() const noexcept { return width; }
		
		int32_t get_height() const noexcept { return height; }

		void set_x(int32_t new_x) noexcept { x = new_x; }
		
		void set_y(int32_t new_y) noexcept { y = new_y; }
		
		void set_width(int32_t new_width) noexcept { width = new_width; }
		
		void set_height(int32_t new_height) noexcept { height = new_height; }

	private:
		
		int32_t x, y;
		
		int32_t width, height;

		xeString name_index;
	};
} // namespace xe is end

#endif // _XE_SURFACE_HPP_ IS END