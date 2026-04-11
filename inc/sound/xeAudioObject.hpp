#ifndef _INC_XE_AUDIO_OBJECT_HPP_
#define _INC_XE_AUDIO_OBJECT_HPP_

#include "audio/xeAudio.hpp"

namespace xe
{
	struct AudioObject
	{
		PcmBlock pcm;

		uint32_t source;
		uint32_t buffer;
		int32_t audioFormat;
		int16_t channels = 1;
		bool loop = false;
	};

} // namespace xe is end

#endif // _INC_XE_AUDIO_OBJECT_HPP_ IS EOF