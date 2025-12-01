#ifndef _INC_FRAGMENTSHADER_HPP_
#define _INC_FRAGMENTSHADER_HPP_

#include "type/xeOrdinals.hpp"
#include "string/xeString.hpp"

namespace xe
{
	class FragmentShader
	{
	public:
		FragmentShader() = default;

		bool load_spirv_shader(byte_t* spirv_data_in, xeString default_entry_function = "main")
		{
			entry_function = default_entry_function;
			spirv_data = spirv_data_in;
		}

	private:

		xeString entry_function;
		
		byte_t* spirv_data;
	};
} // namespace xe is end


#endif // !_INC_FRAGMENTSHADER_HPP_ IS EOF