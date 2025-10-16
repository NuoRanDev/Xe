#ifndef _XE_TESTURE_HPP_
#define _XE_TESTURE_HPP_

#include <any>

namespace xe
{
	class Texture
	{
	public:
		Texture() = default;
		
		~Texture() = default;
	private:

		// The texture object ,such as OpenGL,Vulkan,DirectX and so on.
		std::any texture_obj;
	};
} // namespace xe is end

#endif // !_XE_TESTURE_HPP_ IS EOF