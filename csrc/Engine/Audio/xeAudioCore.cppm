export module xe.Audio.AudioCore;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.xeBaseDataType;

namespace xe
{

	export enum class PcmFormat :xeUint32
	{
		FORMAT_MONO8 = 0x1100,
		FORMAT_MONO16 = 0x1101,
		FORMAT_STEREO8 = 0x1102,
		FORMAT_STEREO16 = 0x1103
	};

	export struct PcmBlock
	{
		xeUint32 freq;
		PcmFormat format;
		xeSize channel;

		xeSize buffer_in;

		xeSize size;
		xeByte* data;

		~PcmBlock()
		{
			xeFree(data);
		}
	};

	export xeSize ReadAudio(void* dst, xeSize size1, xeSize size2, void* class_data);

	export int SeekAudio(void* class_data, xeInt64 to, int type);

	export int CloseAudio(void* class_data);

	export long TellAudio(void* class_data);
}