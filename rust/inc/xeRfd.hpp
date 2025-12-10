#ifndef _INC_XE_RFD_HPP_
#define _INC_XE_RFD_HPP_

#include "string/xeString.hpp"

extern "C"
{
	RustString save_file();

	void free_string(const xe::utf8_t* rust_str);
}

#endif // _INC_XE_RFD_HPP_ IS EOF