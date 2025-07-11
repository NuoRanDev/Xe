export module xe.Audio.AudioCore;

import std;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;
import xe.Core.xeBaseDataType;

namespace xe
{
	export enum class PcmFormat :xeInt32
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

	export enum class PlayState :xeUint32 { _PLAY = 0, _END, _ERROR };

	export xeSize ReadAudioDecodedData(xeAnyTypePtr dst, xeSize type_size, xeSize type_number, xeAnyTypePtr src);

	export int SeekAudioDecodedData(xeAnyTypePtr src, xeInt64 to, int type);

	export int CloseAudioDecodedData(xeAnyTypePtr src);

	export long TellAudioDecodedData(xeAnyTypePtr src);

	export bool IsAudioDecodedDataEOF(xeAnyTypePtr src);
}