export module xe.Audio.AudioCore;

import xe.Core.xeOrdinals;

namespace xe
{
	export enum class PcmFormat :xeUint32
	{
		AL_FORMAT_MONO8 = 0x1100,
		AL_FORMAT_MONO16 = 0x1101,
		AL_FORMAT_STEREO8 = 0x1102,
		AL_FORMAT_STEREO16 = 0x1103
	};

	export struct PcmBlock
	{
		xeInt64 size;
		xeByte* data;
		PcmFormat format;
	};
}