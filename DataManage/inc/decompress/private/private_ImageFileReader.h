#ifndef __PRIVATE__IMAGE_FILE_READER_H_
#define __PRIVATE__IMAGE_FILE_READER_H_

#include "xeDataManageConfig.h"
#include "xeCompressFileStruct.h"

namespace xe
{
	bool DecompressJPEG(Testure* img, byte_t* jpeg_buffer);

	using byte_line = byte_t*;
}

#endif // __PRIVATE__IMAGE_FILE_READER_H_ IS EOF