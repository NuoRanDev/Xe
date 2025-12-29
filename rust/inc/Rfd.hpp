#ifndef _INC_RFD_HPP_
#define _INC_RFD_HPP_

#include "string/xeString.hpp"

extern "C"
{
	RustString rfd_save_file(const xe::utf8_t* xe_str_directory, const xe::utf8_t* xe_str_name, const xe::utf8_t* const* pxe_str_extensions, size_t extensions_number);

	void free_rust_string(const xe::utf8_t* rust_str);
}

#endif // _INC_XE_RFD_HPP_ IS EOF