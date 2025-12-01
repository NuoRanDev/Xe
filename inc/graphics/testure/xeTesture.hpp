#ifndef _XE_TESTURE_HPP_
#define _XE_TESTURE_HPP_

#include "image/xeImage.hpp"

#include "window/xeWindow.hpp"

#include <any>

namespace xe
{
	class Texture
	{
	public:
		Texture(Window *set_bind_window)
		{
			bind_window = set_bind_window;
		}
		
		bool load_image(Image& src_img);

		bool resize_texture(size_t new_width, size_t new_height);

		bool draw(float dst_x, float dst_y, float dst_width, float dst_height);

		~Texture();
	private:

		//
		Window *bind_window;

		// The texture object ,such as OpenGL,Vulkan,DirectX and so on.
		std::any texture_obj;
	};
} // namespace xe is end

#endif // !_XE_TESTURE_HPP_ IS EOF