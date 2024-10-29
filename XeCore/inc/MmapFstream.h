#ifndef _INC_MMAP_F_STREAM_H_
#define _INC_MMAP_F_STREAM_H_

#include "xeCoreConfig.h"

namespace xe
{
	class iMmapfstream;
	class basicMmapfstream;

	class basicMmapfstream
	{
	public:
	private:
		void* pFileView;
	};
	class iMmapfstream
	{};
	class oMmapfstream
	{
	};
}

#endif // !_INC_MMAP_F_STREAM_H_